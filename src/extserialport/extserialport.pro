TARGET = QtExtSerialPort
VERSION = 2.0.0
unix:QT = core
DEFINES += QEXTSERIALPORT_BUILD_SHARED
MODULE_DEFINES += QEXTSERIALPORT_USING_SHARED
QMAKE_DOCS = $$PWD/doc/qtextserialport.qdocconf
load(qt_module)

QMAKE_TARGET_COMPANY = "QtExtSerialPort Project"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2012 QtExtSerialPort Project"

linux*:CONFIG += qesp_linux_udev
include(qextserialport.pri)
