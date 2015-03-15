For: https://codereview.qt-project.org/#change,26305

Error message:
```
main.cpp(1) : fatal error C1083: Cannot open include file: '../../../../../../..
/../../../../../../../../../../../../../../../../../00/aaaaaa/bb/cc/dd/ee/ff/gg/
hh/ii/jj/kk/ll/mm/nn/oo/pp/qq/rr/ss/tt/uu/vv/ww/header.hh': No such file or dire
ctory
```

The following example can be used to reproduce above problem.

All you need to do is make `len(r_header_path) + len(os.getcwd()) + 1 > 259`, for example:

  * change the **common** path
  * change the header file name
  * or change prefix added to **header\_path** or **source\_path**

Note that: run this script under Command Prompt provided by MSVC.

```
#/usr/bin/env python
# -*- coding: UTF-8 -*-

import sys
import os.path

common = "aa/bb/cc/dd/ee/ff/gg/hh/ii/jj/kk/ll/mm/nn/oo/pp/qq/rr/ss/tt/uu/vv/ww/"
uppath = "../../../../../../../../../../../../../../../../../../../../../../../../"

header_path = "00/" + common + "header.hh"
source_path = "11/" + common + "main.cpp"

try:
    os.makedirs(os.path.dirname(header_path))
except (IOError, WindowsError) as e:
    print "I/O error({0}): {1}".format(e.errno, e.strerror)

try:
    os.makedirs(os.path.dirname(source_path))
except (IOError, WindowsError) as e:
    print "I/O error({0}): {1}".format(e.errno, e.strerror)

with file(header_path, 'w') as f:
    f.write("#include<stdio.h>\n")

r_header_path = os.path.join(uppath + header_path)

with file(source_path, 'w') as f:
    f.write('#include"{0}"\n'.format(r_header_path))
    f.write(r"""int main()
    {
        printf("OK, ....\n");
        return 0;
    }""")

os.chdir(os.path.dirname(source_path))
print "*"*79
print "[length ]:", len(r_header_path) + len(os.getcwd()) + 1
print "[cwd    ]:", os.getcwd()
print "[abspath]:", os.path.abspath(r_header_path)
print "[exists ]:", os.path.exists(r_header_path)
print "*"*79

os.system("cl main.cpp")
```