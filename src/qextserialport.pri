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

qextserialport-uselib:!qextserialport-buildlib {
    LIBS += -L$$QEXTSERIALPORT_LIBDIR -l$$QEXTSERIALPORT_LIBNAME
} else {
    HEADERS                += $$PWD/qextserialport.h \
                              $$PWD/qextserialport_p.h \
                              $$PWD/qextserialenumerator.h \
                              $$PWD/qextserialenumerator_p.h \
                              $$PWD/qextserialport_global.h
    SOURCES                += $$PWD/qextserialport.cpp \
                              $$PWD/qextserialenumerator.cpp
    unix:SOURCES           += $$PWD/qextserialport_unix.cpp
    unix:!macx:SOURCES     += $$PWD/qextserialenumerator_unix.cpp
    macx:SOURCES           += $$PWD/qextserialenumerator_osx.cpp
    win32:SOURCES          += $$PWD/qextserialport_win.cpp \
                              $$PWD/qextserialenumerator_win.cpp

    win32:!exists($$[QT_INSTALL_HEADERS]/QtCore/private/qwineventnotifier_p.h){
        DEFINES            += QESP_NO_QT_PRIVATE
        HEADERS            += $$PWD/qextwineventnotifier_p.h
        SOURCES            += $$PWD/qextwineventnotifier_p.cpp
    }
}

macx:LIBS              += -framework IOKit -framework CoreFoundation
win32:LIBS             += -lsetupapi -ladvapi32 -luser32

win32 {
    contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += QEXTSERIALPORT_EXPORT
    else:qextserialport-uselib:!qextserialport-static:DEFINES += QEXTSERIALPORT_IMPORT
}
