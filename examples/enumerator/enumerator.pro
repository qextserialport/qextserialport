######################################################################
# Enumerator
######################################################################


PROJECT = enumerator
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += ../..
QMAKE_LIBDIR += ../../build


OBJECTS_DIR    = obj
MOC_DIR        = moc
UI_DIR         = uic
CONFIG      += qt thread warn_on


SOURCES += main.cpp

LIBS          += -lqextserialport

unix:DEFINES   = _TTY_POSIX_
win32:DEFINES  = _TTY_WIN_ QWT_DLL QT_DLL
