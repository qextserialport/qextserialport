infile(config.pri, QEXTSERIALPORT_LIBRARY, yes): CONFIG += qextserialport-uselib
TEMPLATE += fakelib
QEXTSERIALPORT_LIBNAME = $$qtLibraryTarget(qextserialport-1.2)
TEMPLATE -= fakelib
QEXTSERIALPORT_LIBDIR = $$PWD/lib
unix:qextserialport-uselib:!qextserialport-buildlib:QMAKE_RPATHDIR += $$QEXTSERIALPORT_LIBDIR
