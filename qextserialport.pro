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

DESTDIR                 = lib
TARGET                  = qextserialport
