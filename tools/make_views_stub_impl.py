# Copyright (c) 2016 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

from cef_parser import *
from make_ctocpp_impl import *

def make_views_function_stub_impl(clsname, func):
    name = func.get_name()

    # Build the C++ prototype.
    parts = func.get_cpp_parts(True)
    result = make_ctocpp_impl_proto(clsname, name, func, parts)+' {'

    # Retrieve the function return value.
    retval = func.get_retval()
    retval_type = retval.get_retval_type()
    if retval_type == 'invalid':
        notify(name+' could not be autogenerated')
        # Code could not be auto-generated.
        result += '\n  // COULD NOT IMPLEMENT DUE TO: (return value)'
        result += '\n  #pragma message("Warning: "__FILE__": '+name+' is not implemented")'
        retval_default = ''
    else:
        retval_default = retval.get_retval_default(False)

    result += '\n  NOTIMPLEMENTED();'
    if retval_default != '':
      result += '\n  return ' + retval_default + ';'

    result += '\n}\n\n'

    return result

def make_views_class_stub_impl(header, cls):
    impl = ''

    clsname = cls.get_name()
    funcs = cls.get_static_funcs()
    for func in funcs:
        impl += make_views_function_stub_impl(clsname, func)

    return impl

def make_views_stub_impl(header):
    includes = ''
    impl = ''

    allclasses = header.get_classes()
    for cls in allclasses:
        dir = cls.get_file_directory()
        # Only process files in the views/ directory.
        if dir != None and dir.find('views') == 0:
            cls_impl = make_views_class_stub_impl(header, cls)
            if cls_impl != '':
                impl += cls_impl
                includes += '#include "include/'+cls.get_file_name()+'"\n'

    includes += '\n#include "base/logging.h"\n'

    # Build the final output.
    result = get_copyright() + includes
    result += '\n\n// STATIC STUB METHODS - Do not edit by hand.\n\n'
    result += impl
    return result

def write_views_stub_impl(header, file):
    newcontents = make_views_stub_impl(header)
    return (file, newcontents)

# Test the module.
if __name__ == "__main__":
    import sys

    # Verify that the correct number of command-line arguments are provided.
    if len(sys.argv) < 2:
        sys.stderr.write('Usage: '+sys.argv[0]+' <cpp_header_dir>')
        sys.exit()

    cpp_header_dir = sys.argv[1]

    # Create the header object. Should match the logic in translator.py.
    header = obj_header()
    header.set_root_directory(cpp_header_dir)
    excluded_files = ['cef_application_mac.h', 'cef_version.h']
    header.add_directory(cpp_header_dir, excluded_files)
    header.add_directory(os.path.join(cpp_header_dir, 'views'))

    # Dump the result to stdout.
    sys.stdout.write(make_views_stub_impl(header))
