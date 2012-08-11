TEMPLATE=lib
include(src/qextserialport.pri)
CONFIG += dll

#generate proper library name
SAVE_TEMPLATE = $$TEMPLATE
TEMPLATE = fakelib
contains(QT_VERSION, ^5\\..*\\..*) {
    QEXTSERIALPORT_LIBNAME = $$qtLibraryTarget(QtExtSerialPort-1.2)
} else {
    QEXTSERIALPORT_LIBNAME = $$qtLibraryTarget(qextserialport-1.2)
}
TEMPLATE = $$SAVE_TEMPLATE

TARGET = $$QEXTSERIALPORT_LIBNAME
DEFINES += QEXTSERIALPORT_BUILD_SHARED

mac:CONFIG += absolute_library_soname
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all

win32 {
    DLLDESTDIR = $$[QT_INSTALL_BINS]
    QMAKE_DISTCLEAN += $$[QT_INSTALL_BINS]\\$${QEXTSERIALPORT_LIBNAME}.dll
}

# for make docs
include(doc/doc.pri)

# for make install
target.path = $$[QT_INSTALL_LIBS]
headers.files = src/qextserialport.h \
                src/qextserialenumerator.h \
                src/qextserialport_global.h
headers.path = $$[QT_INSTALL_HEADERS]/QtExtSerialPort
features.files = extserialport.prf
features.path = $$[QMAKE_MKSPECS]/features
INSTALLS += target headers features

