QT += qml extserialport

HEADERS += qextqmlserialport_p.h

SOURCES += plugin.cpp \
           qextqmlserialport.cpp

load(qml_plugin)
