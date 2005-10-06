PROJECT                 = qextserialport
TEMPLATE                = lib
CONFIG                  = warn_on qt thread debug
OBJECTS_DIR             = obj
MOC_DIR                 = moc
HEADERS                 = qextserialbase.h \
                          qextserialport.h
SOURCES                 = qextserialbase.cpp \
                          qextserialport.cpp

unix:HEADERS           += posix_qextserialport.h
unix:SOURCES           += posix_qextserialport.cpp
unix:DEFINES            = _TTY_POSIX_

win32:HEADERS          += win_qextserialport.h
win32:SOURCES          += win_qextserialport.cpp
win32:DEFINES           = _TTY_WIN_

DESTDIR                 = lib
TARGET                  = qextserialport

unix:libraries.files = lib/*
win32:libraries.files = lib/*.*
headers.files = *.h
win32:libraries.path = $(QTDIR)/lib
win32:headers.path = $(QTDIR)/include/qesp
unix:libraries.path = /usr/local/lib
unix:headers.path = /usr/local/include/qesp
INSTALLS += libraries headers
