######################################################################
# QextSerialPort Test Application (QESPTA)
######################################################################


PROJECT = QESPTA
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += ../..
QMAKE_LIBDIR += ../../build


OBJECTS_DIR    = obj
MOC_DIR        = moc
UI_DIR         = uic
CONFIG      += qt thread warn_on


HEADERS += MainWindow.h \
		MessageWindow.h \
		QespTest.h

SOURCES += main.cpp \
		MainWindow.cpp \
		MessageWindow.cpp \
		QespTest.cpp

LIBS          += -lqextserialport

unix:DEFINES   = _TTY_POSIX_
win32:DEFINES  = _TTY_WIN_ QWT_DLL QT_DLL
