PROJECT                 = qextserialport
TEMPLATE                = lib
VERSION                 = 1.2.0
DESTDIR                 = build
CONFIG                 += qt warn_on debug_and_release
CONFIG                  += dll
#CONFIG                 += staticlib

# event driven device enumeration on windows requires the gui module
!win32:QT               -= gui

OBJECTS_DIR             = tmp
MOC_DIR                 = tmp
DEPENDDIR               = .
INCLUDEDIR              = .
HEADERS                 = qextserialport.h \
                          qextserialenumerator.h
SOURCES                 = qextserialport.cpp \
                          qextserialenumerator.cpp

unix:SOURCES           += posix_qextserialport.cpp
macx: LIBS             += -framework IOKit -framework CoreFoundation

win32:SOURCES          += win_qextserialport.cpp
win32:DEFINES          += WINVER=0x0501 # needed for mingw to pull in appropriate dbt business...probably a better way to do this
win32:LIBS             += -lsetupapi

CONFIG(debug, debug|release) {
    TARGET = qextserialportd
} else {
    TARGET = qextserialport
}
