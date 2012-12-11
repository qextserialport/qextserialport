TEMPLATE = app
DEPENDPATH += .
CONFIG += console
include(../../src/extserialport/qextserialport.pri)

SOURCES += main.cpp PortListener.cpp
HEADERS += PortListener.h
