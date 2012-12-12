TARGET = QtExtSerialPort
VERSION = 1.2.0
unix:QT = core
DEFINES += QEXTSERIALPORT_BUILD_SHARED
MODULE_DEFINES += QEXTSERIALPORT_USING_SHARED
#QMAKE_DOCS = $$PWD/doc/qtextserialport.qdocconf
load(qt_module)

linux*:CONFIG += qesp_linux_udev
include(qextserialport.pri)
