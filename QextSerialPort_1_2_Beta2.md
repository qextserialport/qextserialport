

**Hint:** All the information mentioned in [QextSerialPort\_1\_2\_Beta1](QextSerialPort_1_2_Beta1.md) is still valid.

# ChangeLog #
  * [Issue 124](https://code.google.com/p/qextserialport/issues/detail?id=124): implement canReadLine
  * [Issue 122](https://code.google.com/p/qextserialport/issues/detail?id=122): make Dtr default to TRUE under Windows.
  * [Issue 127](https://code.google.com/p/qextserialport/issues/detail?id=127): fix `QObject::MoveToThread` brokes SerialPort on Windows
  * [Issue 129](https://code.google.com/p/qextserialport/issues/detail?id=129): Add custom baud support for Windows.
  * [Issue 131](https://code.google.com/p/qextserialport/issues/detail?id=131): Make sure portName returned by QextSerialEnumerator can be used by QextSerialPort
  * [Issue 134](https://code.google.com/p/qextserialport/issues/detail?id=134) [Issue 12](https://code.google.com/p/qextserialport/issues/detail?id=12): Make "make install" really works
  * [Issue 2](https://code.google.com/p/qextserialport/issues/detail?id=2): device discovery / removal notification on linux (read config\_example.pri to figure out how to enable it.)

# Getting Started #
> The simplest way to play with this library is opening **qextserialport.pro** with Qt Creator.

## Usage(1)：Source Code Only ##

> The package contains a `qextserialport.pri` file that allows you to integrate the component into programs that use qmake for the build step.

All you need is adding following line to your qmake's project file:
```
    include(pathToPri/qextserialport.pri)
```

Then, using QextSerialPort in your code
```
    #include "qextserialport.h"
    ...
    MyClass::MyClass()
    {
        port = new QextSerialPort("COM1");
        connect(port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
        port->open();
    }

    void MyClass::onDataAvailable()
    {
        QByteArray data = port->readAll();
        processNewData(usbdata);
    }
```

**Hint:** If you like, you can copy all files from qextserialport/src to your application's source path. then add following line to your project file
```
include(qextserialport.pri)
```

## Usage(2)：System Wide Library ##
> Although QextSerialPort can be directly compiled into your application, you may prefer to use QextSerailPort as an shared library, which is very easy too.

1. Goto the sub directory **buildlib**, and run

```
qmake
make
sudo make install
```

The library, the header files, and the feature file will be installed to your system.

2. Add following line to your qmake's project file:

```
CONFIG += extserialport
```

3. Then, using QextSerialPort in your code
```
    #include "qextserialport.h"
```

**Not familiar with how to use share library?** Perhaps this is useful for you [HowToUseLibrary](HowToUseLibrary.md)

## Usage(3)：Shared or Static Library Without Installation ##
If I don't want to install the library to my system. How should I do?

It's easy too. ;-)

1. 【Config】 Write a **config.pri** file.(read config\_example.pri for reference):

  * shared library
  * static library

2. 【Build】 Changed to subdirectory **buildlib**, run

```
qmake
make
```

shared or static library will be generated.

3. 【Usage】 Add following line to your qmake project file:

```
# xxxx.pro
include(pathToPri/qextserialport.pri)
```

**Hint:** If you just want to build QextSerialPort as shared library, and manually set INCLUDEPATH and LIBS in your project's file, the config.pri file can be omitted.

```
# xxxx.pro
INCLUDEPATH += /home/xxxx/download/qextserialport/src
LIBS += -L/home/xxxx/download/qextserialport/lib -lqextserialport-1.2
DEFINES += QEXTSERIALPORT_USING_SHARED
```

# Implementation Details #

## How does QextSerialPort work? ##
  * The class QextSerialPort consists of the following five files.

|qextserialport.h  |public header file |
|:-----------------|:------------------|
|qextserialport\_p.h|private header file|
|qextserialport.cpp|common implementation and documentation|
|qextserialport\_win.cpp |Windows special implementation|
|qextserialport\_unix.cpp|Posix special implementation|

  * Common IO operation:

| | |Windows |Posix |
|:|:|:-------|:-----|
|QextSerialPort::open()|QextSerialPortPrivate::open\_sys()|::CreateFileW() |::open()|
|QextSerialPort::close()|QextSerialPortPrivate::close\_sys()|::CloseHandle() |::close()|
|QextSerialPort::flush()|QextSerialPortPrivate::flush\_sys()|::FlushFileBuffers() |::tcdrain()|
|QextSerialPort::read()|QextSerialPortPrivate::readData\_sys()|::ReadFile() |::read()|
|QextSerialPort::write()|QextSerialPortPrivate::writeData\_sys()|::WriteFile() |::write()|

  * Serial Port Settings

Part1:

| | |Windows |Posix |
|:|:|:-------|:-----|
|QextSerialPort::setBaudRate()<br />QextSerialPort:setDataBits();<br />QextSerialPort:setParity()<br />QextSerialPort:setStopBits()<br />QextSerialPort:setFlowControl()|QextSerialPortPrivate::setBaudRate()<br />QextSerialPortPrivate::setDataBits();<br />QextSerialPortPrivate::setParity()<br />QextSerialPortPrivate::setStopBits()<br />QextSerialPortPrivate::setFlowControl()|Fill struct:<br /> COMMCONFIG<br />(mainly: DCB)|Fill struct:<br /> termios |
|  |QextSerialPortPrivate::updatePortSettings()|::SetCommConfig() |::tcsetattr()|

Part2:

| | |Windows |Posix |
|:|:|:-------|:-----|
|QextSerialPort::setTimeout()|QextSerialPortPrivate::setTimeout()|Fill struct:<br /> COMMTIMEOUTS |Fill struct:<br /> termios |
|  |QextSerialPortPrivate::updatePortSettings()|::SetCommTimeouts() |::tcsetattr()|

**Note**:All the settings are saved in QextSerialPortPrivate, and the fill operations are done in QextSerialPortPrivate::updatePortSettings() when port is opened.

  * Other Operation:

| | |Windows | Posix|
|:|:|:-------|:-----|
|QextSerialPort::setDtr()|QextSerialPortPrivate::setDtr\_sys() |::EscapeCommFunction()|::ioctl()|
|QextSerialPort::setRts()|QextSerialPortPrivate::setRts\_sys()|::EscapeCommFunction()|::ioctl()|


## How does QextSerialEnumerator work? ##
  * The class QextSerialEnumerator consists of the following seven files.

|qextserialenumerator.h  |public header file |
|:-----------------------|:------------------|
|qextserialenumerator\_p.h|private header file|
|qextserialenumerator.cpp|common implementation and documentation|
|qextserialenumerator\_win.cpp |Windows special implementation|
|qextserialenumerator\_osx.cpp|Mac special implementation|
|qextserialenumerator\_linux.cpp|linux special implementation(with  libudev)|
|qextserialenumerator\_unix.cpp|Posix special implementation, not contained by above `_osx.cpp and _linux.cpp`|

### getPort() ###

  * QextSerialEnumeratorPrivate::getPorts\_sys()

It's too platform special to list a table here. If you are interested in this class, read the source code please ;-)