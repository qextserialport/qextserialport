

<font color='red'>Note: This version has not released yet. Though you can get source code from repository</font>

# Getting Started #

**Hint:** You can use QtCeator to open one or all of the examples provided by the library. Feel free to play with them.

## Usage(1):Source Code Only ##

> The package contains a `qextserialport.pri` file that allows you to integrate the component into programs that use qmake for the build step.

  * Download the source code.

  * Put the source code in any directory you like. For example, 3rdparty:
```
|-- project.pro
|-- ....
|-- 3rdparty\
|     |-- qextserialport\
|     |
```

  * Add following line to your qmake project file:
```
include(3rdparty/qextserialport/src/qextserialport.pri)
```

  * Then, using QextSerialPort in your code
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
        processNewData(data);
    }
```

**Hint:** If you like, you can copy all files from qextserialport/src to your application's source path. then add following line to your project file
```
include(qextserialport.pri)
```

## Usage(2):Shared Library ##
> Although QextSerialPort can be directly compiled into your application, you may prefer to use QextSerailPort as a shared library, which is very easy too.

  * Goto the top level directory , and run
```
qmake
sudo make install
```

The library, the header files, and the feature file will be installed to your system.

  * Add following line to your qmake's project file:
```
CONFIG += extserialport
```

  * Then, using QextSerialPort in your code
```
    #include "qextserialport.h"
```

**Note:** If you are not using qmake and Qt Creator, the macro QEXTSERIALPORT\_USING\_SHARED should be defined manully.

**Not familiar with how to use share library?** Perhaps this is useful for you [HowToUseLibrary](HowToUseLibrary.md)

## Usage(3):Static Library ##
Someone prefer to use QextSerailPort as static library.

  * Open the project file: **qextserialport.pro**, add uncomment follow line
```
# CONFIG += qesp_static
```

  * Then follow the same steps as shared library
```
qmake
sudo make install
```
The static library, the header files, and the feature file will be installed to your system.

  * Add following line to your qmake's project file:
```
CONFIG += extserialport
```

# Platform Special #

## For MacX: Build as framework ##

  * Open the project file: **qextserialport.pro**, and uncomment follow line
```
# CONFIG += qesp_mac_framework
```

  * Then follow the same steps as shared library

  * Goto the top level directory , and run
```
qmake
sudo make install
```

The framework which includes libraries and the header files, and the feature file will be installed to your system.

  * Add following line to your qmake's project file:
```
CONFIG += extserialport
```

## For Linux: Enable udev ##
  * Open the project file: **qextserialport.pro**, uncomment follow line
```
#linux*:CONFIG += qesp_linux_udev
```

  * Note, If you are using the usage(1), Add following line
```
CONFIG += qesp_linux_udev
```
before include the qextserialport.pri file.

# Implementation Details #
## Why include "moc\_qextserialport.cpp" in "qextserialport.cpp"? ##

In order to move private data member and private slots to non-public files, d-pointer and Q\_PRIVATE\_SLOT are used. So the generated moc\_qextserialport.cpp is dependent on non-public files, which make it can not be compiled as a compile unit any more.

More information can be found in [Three\_Usage\_Of\_MOC](Three_Usage_Of_MOC.md)


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
  * The class QextSerialEnumerator consists of the following six files.

|qextserialenumerator.h  |public header file |
|:-----------------------|:------------------|
|qextserialenumerator\_p.h|private header file|
|qextserialenumerator.cpp|common implementation and documentation|
|qextserialenumerator\_win.cpp |Windows special implementation|
|qextserialenumerator\_unix.cpp|Unix special implementation|
|qextserialenumerator\_osx.cpp|Mac special implementation|

### getPort() ###

  * QextSerialEnumeratorPrivate::getPorts\_sys()

It's too platform special to list a table here. If you are interested in this class, read the source code please ;-)