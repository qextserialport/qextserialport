exists(../common.pri) {
    #For case:
    #  someone want to copy all file in the src/ directory
    #  to their project src/ directory and they does not like
    #  the common.pri file.
    #In this case:
    #  they can just include this file (qextserialport.pri) too.
    include(../common.pri)
}
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

qextserialport-library:!qextserialport-buildlib {
    # Using QextSerialPort as shared or static library.
    LIBS += -L$$QEXTSERIALPORT_LIBDIR -l$$QEXTSERIALPORT_LIBNAME
   !qextserialport-static: DEFINES += QEXTSERIALPORT_USING_SHARED
} else {
    # Building library(shared or static)
    # or including source files
    HEADERS                += $$PWD/qextserialport.h \
                              $$PWD/qextserialport_p.h \
                              $$PWD/qextserialenumerator.h \
                              $$PWD/qextserialenumerator_p.h \
                              $$PWD/qextserialport_global.h
    SOURCES                += $$PWD/qextserialport.cpp \
                              $$PWD/qextserialenumerator.cpp
    unix {
        SOURCES            += $$PWD/qextserialport_unix.cpp
        linux*:!qextserialport-no-udev {
            SOURCES        += $$PWD/qextserialenumerator_linux.cpp
        } else:macx {
            SOURCES        += $$PWD/qextserialenumerator_osx.cpp
        } else {
            SOURCES        += $$PWD/qextserialenumerator_unix.cpp
        }
    }
    win32:SOURCES          += $$PWD/qextserialport_win.cpp \
                              $$PWD/qextserialenumerator_win.cpp

    # For Windows user who doesn't have Qt4's Private files
    win32:contains(QT_VERSION, ^4\\..*\\..*):!exists($$[QT_INSTALL_HEADERS]/QtCore/private/qwineventnotifier_p.h){
        DEFINES            += QESP_NO_QT4_PRIVATE
        HEADERS            += $$PWD/qextwineventnotifier_p.h
        SOURCES            += $$PWD/qextwineventnotifier_p.cpp
    }

    # For building shared library only
    qextserialport-buildlib:contains(TEMPLATE, .*lib):contains(CONFIG, shared){
        DEFINES += QEXTSERIALPORT_BUILD_SHARED
    }

    linux*:qextserialport-no-udev:DEFINES += QESP_NO_UDEV
}

macx:LIBS              += -framework IOKit -framework CoreFoundation
win32:LIBS             += -lsetupapi -ladvapi32 -luser32
linux*:!qextserialport-no-udev: LIBS += -ludev

# moc doesn't detect Q_OS_LINUX correctly, so add this to make it work
linux*:DEFINES += __linux__
