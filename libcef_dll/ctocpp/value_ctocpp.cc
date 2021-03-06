// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=dce21cc429320a9d24fd80ad2bc1347b03f82808$
//

#include "libcef_dll/ctocpp/value_ctocpp.h"
#include "libcef_dll/ctocpp/binary_value_ctocpp.h"
#include "libcef_dll/ctocpp/dictionary_value_ctocpp.h"
#include "libcef_dll/ctocpp/list_value_ctocpp.h"

// STATIC METHODS - Body may be edited by hand.

CefRefPtr<CefValue> CefValue::Create() {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_value_t* _retval = cef_value_create();

  // Return type: refptr_same
  return CefValueCToCpp::Wrap(_retval);
}

// VIRTUAL METHODS - Body may be edited by hand.

bool CefValueCToCpp::IsValid() {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_valid))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_valid(_struct);

  // Return type: bool
  return _retval ? true : false;
}

bool CefValueCToCpp::IsOwned() {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_owned))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_owned(_struct);

  // Return type: bool
  return _retval ? true : false;
}

bool CefValueCToCpp::IsReadOnly() {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_read_only))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_read_only(_struct);

  // Return type: bool
  return _retval ? true : false;
}

bool CefValueCToCpp::IsSame(CefRefPtr<CefValue> that) {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_same))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: that; type: refptr_same
  DCHECK(that.get());
  if (!that.get())
    return false;

  // Execute
  int _retval = _struct->is_same(_struct, CefValueCToCpp::Unwrap(that));

  // Return type: bool
  return _retval ? true : false;
}

bool CefValueCToCpp::IsEqual(CefRefPtr<CefValue> that) {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_equal))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: that; type: refptr_same
  DCHECK(that.get());
  if (!that.get())
    return false;

  // Execute
  int _retval = _struct->is_equal(_struct, CefValueCToCpp::Unwrap(that));

  // Return type: bool
  return _retval ? true : false;
}

CefRefPtr<CefValue> CefValueCToCpp::Copy() {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, copy))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_value_t* _retval = _struct->copy(_struct);

  // Return type: refptr_same
  return CefValueCToCpp::Wrap(_retval);
}

CefValueType CefValueCToCpp::GetType() {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_type))
    return VTYPE_INVALID;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_value_type_t _retval = _struct->get_type(_struct);

  // Return type: simple
  return _retval;
}

bool CefValueCToCpp::GetBool() {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_bool))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->get_bool(_struct);

  // Return type: bool
  return _retval ? true : false;
}

int CefValueCToCpp::GetInt() {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_int))
    return 0;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->get_int(_struct);

  // Return type: simple
  return _retval;
}

double CefValueCToCpp::GetDouble() {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_double))
    return 0;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  double _retval = _struct->get_double(_struct);

  // Return type: simple
  return _retval;
}

CefString CefValueCToCpp::GetString() {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_string))
    return CefString();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_string_userfree_t _retval = _struct->get_string(_struct);

  // Return type: string
  CefString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

CefRefPtr<CefBinaryValue> CefValueCToCpp::GetBinary() {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_binary))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_binary_value_t* _retval = _struct->get_binary(_struct);

  // Return type: refptr_same
  return CefBinaryValueCToCpp::Wrap(_retval);
}

CefRefPtr<CefDictionaryValue> CefValueCToCpp::GetDictionary() {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_dictionary))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_dictionary_value_t* _retval = _struct->get_dictionary(_struct);

  // Return type: refptr_same
  return CefDictionaryValueCToCpp::Wrap(_retval);
}

CefRefPtr<CefListValue> CefValueCToCpp::GetList() {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_list))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_list_value_t* _retval = _struct->get_list(_struct);

  // Return type: refptr_same
  return CefListValueCToCpp::Wrap(_retval);
}

bool CefValueCToCpp::SetNull() {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_null))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->set_null(_struct);

  // Return type: bool
  return _retval ? true : false;
}

bool CefValueCToCpp::SetBool(bool value) {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_bool))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->set_bool(_struct, value);

  // Return type: bool
  return _retval ? true : false;
}

bool CefValueCToCpp::SetInt(int value) {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_int))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->set_int(_struct, value);

  // Return type: bool
  return _retval ? true : false;
}

bool CefValueCToCpp::SetDouble(double value) {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_double))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->set_double(_struct, value);

  // Return type: bool
  return _retval ? true : false;
}

bool CefValueCToCpp::SetString(const CefString& value) {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_string))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Unverified params: value

  // Execute
  int _retval = _struct->set_string(_struct, value.GetStruct());

  // Return type: bool
  return _retval ? true : false;
}

bool CefValueCToCpp::SetBinary(CefRefPtr<CefBinaryValue> value) {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_binary))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: value; type: refptr_same
  DCHECK(value.get());
  if (!value.get())
    return false;

  // Execute
  int _retval =
      _struct->set_binary(_struct, CefBinaryValueCToCpp::Unwrap(value));

  // Return type: bool
  return _retval ? true : false;
}

bool CefValueCToCpp::SetDictionary(CefRefPtr<CefDictionaryValue> value) {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_dictionary))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: value; type: refptr_same
  DCHECK(value.get());
  if (!value.get())
    return false;

  // Execute
  int _retval =
      _struct->set_dictionary(_struct, CefDictionaryValueCToCpp::Unwrap(value));

  // Return type: bool
  return _retval ? true : false;
}

bool CefValueCToCpp::SetList(CefRefPtr<CefListValue> value) {
  cef_value_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_list))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: value; type: refptr_same
  DCHECK(value.get());
  if (!value.get())
    return false;

  // Execute
  int _retval = _struct->set_list(_struct, CefListValueCToCpp::Unwrap(value));

  // Return type: bool
  return _retval ? true : false;
}

// CONSTRUCTOR - Do not edit by hand.

CefValueCToCpp::CefValueCToCpp() {}

template <>
cef_value_t*
CefCToCppRefCounted<CefValueCToCpp, CefValue, cef_value_t>::UnwrapDerived(
    CefWrapperType type,
    CefValue* c) {
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template <>
base::AtomicRefCount
    CefCToCppRefCounted<CefValueCToCpp, CefValue, cef_value_t>::DebugObjCt = 0;
#endif

template <>
CefWrapperType
    CefCToCppRefCounted<CefValueCToCpp, CefValue, cef_value_t>::kWrapperType =
        WT_VALUE;
