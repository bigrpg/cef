// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "libcef/browser/browser_context_impl.h"

#include <map>

#include "libcef/browser/browser_context_proxy.h"
#include "libcef/browser/context.h"
#include "libcef/browser/download_manager_delegate.h"
#include "libcef/browser/permission_manager.h"
#include "libcef/browser/ssl_host_state_delegate.h"
#include "libcef/browser/thread_util.h"
#include "libcef/common/cef_switches.h"
#include "libcef/common/extensions/extensions_util.h"

#include "base/command_line.h"
#include "base/files/file_util.h"
#include "base/lazy_instance.h"
#include "base/logging.h"
#include "base/prefs/pref_service.h"
#include "base/strings/string_util.h"
#include "base/threading/thread_restrictions.h"
#include "chrome/browser/net/proxy_service_factory.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/guest_view/browser/guest_view_manager.h"
#include "content/public/browser/download_manager.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/storage_partition.h"
#include "net/proxy/proxy_config_service.h"

using content::BrowserThread;

namespace {

// Manages the global list of Impl instances.
class ImplManager {
 public:
  typedef std::vector<CefBrowserContextImpl*> Vector;

  ImplManager() {}
  ~ImplManager() {
    DCHECK(all_.empty());
    DCHECK(map_.empty());
  }

  void AddImpl(CefBrowserContextImpl* impl) {
    CEF_REQUIRE_UIT();
    DCHECK(!IsValidImpl(impl));
    all_.push_back(impl);
  }

  void RemoveImpl(CefBrowserContextImpl* impl, const base::FilePath& path) {
    CEF_REQUIRE_UIT();

    Vector::iterator it = GetImplPos(impl);
    DCHECK(it != all_.end());
    all_.erase(it);

    if (!path.empty()) {
      PathMap::iterator it = map_.find(path);
      DCHECK(it != map_.end());
      if (it != map_.end())
        map_.erase(it);
    }
  }

  bool IsValidImpl(const CefBrowserContextImpl* impl) {
    CEF_REQUIRE_UIT();
    return GetImplPos(impl) != all_.end();
  }

  CefBrowserContextImpl* GetImplForContext(
      const content::BrowserContext* context) {
    CEF_REQUIRE_UIT();

    Vector::iterator it = all_.begin();
    for (; it != all_.end(); ++it) {
      if (*it == context || (*it)->HasProxy(context))
        return *it;
    }
    return NULL;
  }

  void SetImplPath(CefBrowserContextImpl* impl, const base::FilePath& path) {
    CEF_REQUIRE_UIT();
    DCHECK(!path.empty());
    DCHECK(IsValidImpl(impl));
    DCHECK(GetImplForPath(path) == NULL);
    map_.insert(std::make_pair(path, impl));
  }

  CefBrowserContextImpl* GetImplForPath(const base::FilePath& path) {
    CEF_REQUIRE_UIT();
    DCHECK(!path.empty());
    PathMap::const_iterator it = map_.find(path);
    if (it != map_.end())
      return it->second;
    return NULL;
  }

  const Vector GetAllImpl() const { return all_; }

 private:
  Vector::iterator GetImplPos(const CefBrowserContextImpl* impl) {
    Vector::iterator it = all_.begin();
    for (; it != all_.end(); ++it) {
      if (*it == impl)
        return it;
    }
    return all_.end();
  }

  typedef std::map<base::FilePath, CefBrowserContextImpl*> PathMap;
  PathMap map_;

  Vector all_;

  DISALLOW_COPY_AND_ASSIGN(ImplManager);
};

base::LazyInstance<ImplManager> g_manager = LAZY_INSTANCE_INITIALIZER;

}  // namespace

CefBrowserContextImpl::CefBrowserContextImpl(
    const CefRequestContextSettings& settings)
    : settings_(settings) {
  g_manager.Get().AddImpl(this);
}

CefBrowserContextImpl::~CefBrowserContextImpl() {
  pref_proxy_config_tracker_->DetachFromPrefService();

  if (host_content_settings_map_.get())
    host_content_settings_map_->ShutdownOnUIThread();

  // Delete the download manager delegate here because otherwise we'll crash
  // when it's accessed from the content::BrowserContext destructor.
  if (download_manager_delegate_.get())
    download_manager_delegate_.reset(NULL);

  g_manager.Get().RemoveImpl(this, cache_path_);
}

void CefBrowserContextImpl::Initialize() {
  cache_path_ = base::FilePath(CefString(&settings_.cache_path));
  if (!cache_path_.empty()) {
    base::ThreadRestrictions::ScopedAllowIO allow_io;
    if (!base::DirectoryExists(cache_path_) &&
        !base::CreateDirectory(cache_path_)) {
      LOG(ERROR) << "The cache_path directory could not be created: " <<
          cache_path_.value();
      cache_path_ = base::FilePath();
      CefString(&settings_.cache_path).clear();
    }
  }

  if (!cache_path_.empty())
    g_manager.Get().SetImplPath(this, cache_path_);

  if (settings_.accept_language_list.length == 0) {
    // Use the global language list setting.
    CefString(&settings_.accept_language_list) =
        CefString(&CefContext::Get()->settings().accept_language_list);
  }

  // Initialize user preferences.
  pref_store_ = new CefBrowserPrefStore();
  pref_store_->SetInitializationCompleted();
  pref_service_ = pref_store_->CreateService().Pass();

  CefBrowserContext::Initialize();

  // Initialize proxy configuration tracker.
  pref_proxy_config_tracker_.reset(
      ProxyServiceFactory::CreatePrefProxyConfigTrackerOfLocalState(
          GetPrefs()));

  // Create the CefURLRequestContextGetterImpl via an indirect call to
  // CreateRequestContext. Triggers a call to CefURLRequestContextGetterImpl::
  // GetURLRequestContext() on the IO thread which creates the
  // CefURLRequestContextImpl.
  GetRequestContext();
  DCHECK(url_request_getter_.get());
}

void CefBrowserContextImpl::AddProxy(const CefBrowserContextProxy* proxy) {
  CEF_REQUIRE_UIT();
  DCHECK(!HasProxy(proxy));
  proxy_list_.push_back(proxy);
}

void CefBrowserContextImpl::RemoveProxy(const CefBrowserContextProxy* proxy) {
  CEF_REQUIRE_UIT();
  bool found = false;
  ProxyList::iterator it = proxy_list_.begin();
  for (; it != proxy_list_.end(); ++it) {
    if (*it == proxy) {
      proxy_list_.erase(it);
      found = true;
      break;
    }
  }
  DCHECK(found);
}

bool CefBrowserContextImpl::HasProxy(
    const content::BrowserContext* context) const {
  CEF_REQUIRE_UIT();
  ProxyList::const_iterator it = proxy_list_.begin();
  for (; it != proxy_list_.end(); ++it) {
    if (*it == context)
      return true;
  }
  return false;
}

// static
scoped_refptr<CefBrowserContextImpl> CefBrowserContextImpl::GetForCachePath(
    const base::FilePath& cache_path) {
  return g_manager.Get().GetImplForPath(cache_path);
}

// static
CefRefPtr<CefBrowserContextImpl> CefBrowserContextImpl::GetForContext(
    content::BrowserContext* context) {
  return g_manager.Get().GetImplForContext(context);
}

// static
std::vector<CefBrowserContextImpl*> CefBrowserContextImpl::GetAll() {
  return g_manager.Get().GetAllImpl();
}

base::FilePath CefBrowserContextImpl::GetPath() const {
  return cache_path_;
}

scoped_ptr<content::ZoomLevelDelegate>
    CefBrowserContextImpl::CreateZoomLevelDelegate(const base::FilePath&) {
  return scoped_ptr<content::ZoomLevelDelegate>();
}

bool CefBrowserContextImpl::IsOffTheRecord() const {
  return cache_path_.empty();
}

content::DownloadManagerDelegate*
    CefBrowserContextImpl::GetDownloadManagerDelegate() {
  DCHECK(!download_manager_delegate_.get());

  content::DownloadManager* manager = BrowserContext::GetDownloadManager(this);
  download_manager_delegate_.reset(new CefDownloadManagerDelegate(manager));
  return download_manager_delegate_.get();
}

net::URLRequestContextGetter* CefBrowserContextImpl::GetRequestContext() {
  CEF_REQUIRE_UIT();
  return GetDefaultStoragePartition(this)->GetURLRequestContext();
}

net::URLRequestContextGetter*
    CefBrowserContextImpl::GetRequestContextForRenderProcess(
        int renderer_child_id) {
  return GetRequestContext();
}

net::URLRequestContextGetter*
    CefBrowserContextImpl::GetMediaRequestContext() {
  return GetRequestContext();
}

net::URLRequestContextGetter*
    CefBrowserContextImpl::GetMediaRequestContextForRenderProcess(
        int renderer_child_id)  {
  return GetRequestContext();
}

net::URLRequestContextGetter*
    CefBrowserContextImpl::GetMediaRequestContextForStoragePartition(
        const base::FilePath& partition_path,
        bool in_memory) {
  return GetRequestContext();
}

content::BrowserPluginGuestManager* CefBrowserContextImpl::GetGuestManager() {
  DCHECK(extensions::ExtensionsEnabled());
  return guest_view::GuestViewManager::FromBrowserContext(this);
}

storage::SpecialStoragePolicy*
    CefBrowserContextImpl::GetSpecialStoragePolicy() {
  return NULL;
}

content::PushMessagingService*
    CefBrowserContextImpl::GetPushMessagingService() {
  return NULL;
}

content::SSLHostStateDelegate*
    CefBrowserContextImpl::GetSSLHostStateDelegate() {
  if (!ssl_host_state_delegate_.get())
    ssl_host_state_delegate_.reset(new CefSSLHostStateDelegate());
  return ssl_host_state_delegate_.get();
}

content::PermissionManager* CefBrowserContextImpl::GetPermissionManager() {
  if (!permission_manager_.get())
    permission_manager_.reset(new CefPermissionManager());
  return permission_manager_.get();
}

const CefRequestContextSettings& CefBrowserContextImpl::GetSettings() const {
  return settings_;
}

CefRefPtr<CefRequestContextHandler> CefBrowserContextImpl::GetHandler() const {
  return NULL;
}

net::URLRequestContextGetter* CefBrowserContextImpl::CreateRequestContext(
    content::ProtocolHandlerMap* protocol_handlers,
    content::URLRequestInterceptorScopedVector request_interceptors) {
  CEF_REQUIRE_UIT();
  DCHECK(!url_request_getter_.get());

  // Initialize the proxy configuration service.
  scoped_ptr<net::ProxyConfigService> proxy_config_service(
      ProxyServiceFactory::CreateProxyConfigService(
          pref_proxy_config_tracker_.get()));

  url_request_getter_ = new CefURLRequestContextGetterImpl(
      settings_,
      BrowserThread::UnsafeGetMessageLoopForThread(BrowserThread::IO),
      BrowserThread::UnsafeGetMessageLoopForThread(BrowserThread::FILE),
      protocol_handlers,
      proxy_config_service.Pass(),
      request_interceptors.Pass());
  resource_context()->set_url_request_context_getter(url_request_getter_.get());
  return url_request_getter_.get();
}

net::URLRequestContextGetter*
    CefBrowserContextImpl::CreateRequestContextForStoragePartition(
        const base::FilePath& partition_path,
        bool in_memory,
        content::ProtocolHandlerMap* protocol_handlers,
        content::URLRequestInterceptorScopedVector request_interceptors) {
  return NULL;
}

HostContentSettingsMap* CefBrowserContextImpl::GetHostContentSettingsMap() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (!host_content_settings_map_.get()) {
    // The |incognito| argument is intentionally set to false as it otherwise
    // limits the types of values that can be stored in the settings map (for
    // example, default values set via DefaultProvider::SetWebsiteSetting).
    host_content_settings_map_ = new HostContentSettingsMap(GetPrefs(), false);

    // Change the default plugin policy.
    const base::CommandLine* command_line =
        base::CommandLine::ForCurrentProcess();
    const std::string& plugin_policy_str =
      command_line->GetSwitchValueASCII(switches::kPluginPolicy);
    if (!plugin_policy_str.empty()) {
      ContentSetting plugin_policy = CONTENT_SETTING_ALLOW;
      if (base::LowerCaseEqualsASCII(plugin_policy_str,
                                     switches::kPluginPolicy_Detect)) {
        plugin_policy = CONTENT_SETTING_DETECT_IMPORTANT_CONTENT;
      } else if (base::LowerCaseEqualsASCII(plugin_policy_str,
                                            switches::kPluginPolicy_Block)) {
        plugin_policy = CONTENT_SETTING_BLOCK;
      }
      host_content_settings_map_->SetDefaultContentSetting(
          CONTENT_SETTINGS_TYPE_PLUGINS, plugin_policy);
    }
  }
  return host_content_settings_map_.get();
}

PrefService* CefBrowserContextImpl::GetPrefs() {
  return pref_service_.get();
}
