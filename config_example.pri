# This is an config for building and using qextserialport library.
# You can delete this file if you do not use qextserialport as an 
# shared or static libaray.
# 
# When using the qextserialport, all you need is add following line
# in your project.pro file:
#    include(pathToQextserialport/src/qextserialport.pri)
# no matter your use qextserialport as shatic library, shared library
# or just include source files in your project.
#
# uncomment QEXTSERIALPORT_LIBRARY if you want to use this as library
# default is shared library, uncomment QEXTSERIALPORT_STATIC if you 
# want to use static library
QEXTSERIALPORT_LIBRARY = yes
#QEXTSERIALPORT_STATIC = yes
