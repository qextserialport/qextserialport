TEMPLATE = app
DEPENDPATH += .
CONFIG += console
include(../../../src/extserialport/qextserialport.pri)
SOURCES += main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/extserialport/enumerator
INSTALLS += target
