TEMPLATE=lib
include(src/qextserialport.pri)
#create_prl is needed,
#otherwise, MinGW can't found qextserialport1.a
CONFIG += dll create_prl

#generate proper library name
greaterThan(QT_MAJOR_VERSION, 4) {
    QEXTSERIALPORT_LIBNAME = $$qtLibraryTarget(QtExtSerialPort)
} else {
    QEXTSERIALPORT_LIBNAME = $$qtLibraryTarget(qextserialport)
}

TARGET = $$QEXTSERIALPORT_LIBNAME
DEFINES += QEXTSERIALPORT_BUILD_SHARED
VERSION = 1.2.0
mac:CONFIG += absolute_library_soname
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all

win32 {
    DLLDESTDIR = $$[QT_INSTALL_BINS]
    QMAKE_DISTCLEAN += $$[QT_INSTALL_BINS]\\$${QEXTSERIALPORT_LIBNAME}.dll
}

# generate feature file by qmake based on this *.in file.
QMAKE_SUBSTITUTES += extserialport.prf.in

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

