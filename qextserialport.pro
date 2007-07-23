PROJECT                 = qextserialport
TEMPLATE                = lib

CONFIG                 -= debug_and_release
#CONFIG                 += debug
CONFIG                 += release
CONFIG                 += warn_on qt thread
QT                     -= gui

OBJECTS_DIR             = build/obj
MOC_DIR                 = build/moc
DEPENDDIR               = .
INCLUDEDIR              = .
HEADERS                 = qextserialbase.h \
                          qextserialport.h \
                          qextserialenumerator.h
SOURCES                 = qextserialbase.cpp \
                          qextserialport.cpp \
                          qextserialenumerator.cpp

unix:HEADERS           += posix_qextserialport.h
unix:SOURCES           += posix_qextserialport.cpp
unix:DEFINES           += _TTY_POSIX_

win32:HEADERS          += win_qextserialport.h
win32:SOURCES          += win_qextserialport.cpp
win32:DEFINES          += _TTY_WIN_
win32:LIBS             += -lsetupapi

DESTDIR                 = build
#DESTDIR					= examples/enumerator/debug
TARGET                  = qextserialport
unix:VERSION            = 1.2.0
