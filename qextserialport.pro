# Uncomment following line if build static library
# CONFIG += qesp_static

# Uncomment following line if build framework for mac
# macx:CONFIG += qesp_mac_framework

# Uncomment following line if enable udev for linux
# linux*:CONFIG += qesp_linux_udev

######################################################

TEMPLATE=lib
include(src/qextserialport.pri)

#create_prl is needed,
#otherwise, MinGW can't found libqextserialport1.a
CONFIG += create_prl

#mac framework is designed for shared library
macx:qesp_mac_framework: CONFIG -= qesp_static

qesp_static {
    CONFIG += static
} else {
    CONFIG += shared
    macx:CONFIG += absolute_library_soname
    DEFINES += QEXTSERIALPORT_BUILD_SHARED
}
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all

#generate proper library name
greaterThan(QT_MAJOR_VERSION, 4) {
    QESP_LIBNAME = $$qtLibraryTarget(QtExtSerialPort)
} else {
    QESP_LIBNAME = $$qtLibraryTarget(qextserialport)
}

TARGET = $$QESP_LIBNAME
VERSION = 1.2.0

# generate feature file by qmake based on this *.in file.
QMAKE_SUBSTITUTES += extserialport.prf.in

# for make docs
include(doc/doc.pri)

# for make install
win32:!qesp_static {
   dlltarget.path = $$[QT_INSTALL_BINS]
   INSTALLS += dlltarget
}
target.path = $$[QT_INSTALL_LIBS]
headers.files = src/qextserialport.h \
                src/qextserialenumerator.h \
                src/qextserialport_global.h
headers.path = $$[QT_INSTALL_HEADERS]/QtExtSerialPort
features.files = extserialport.prf
features.path = $$[QMAKE_MKSPECS]/features
INSTALLS += target headers features

