infile(config.pri, QEXTSERIALPORT_LIBRARY, yes): CONFIG += qextserialport-uselib
qextserialport-uselib{
infile(config.pri, QEXTSERIALPORT_STATIC, yes): CONFIG += qextserialport-static
}
TEMPLATE += fakelib
QEXTSERIALPORT_LIBNAME = $$qtLibraryTarget(qextserialport-1.2)
TEMPLATE -= fakelib
QEXTSERIALPORT_LIBDIR = $$PWD/lib
unix:qextserialport-uselib:!qextserialport-buildlib:QMAKE_RPATHDIR += $$QEXTSERIALPORT_LIBDIR
