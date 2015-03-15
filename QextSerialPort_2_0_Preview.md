

> <font color='red'>Note: This version has not released yet. Though you can get source code from repository.</font>

QextSerailPort 2.0 can be built in two different ways:

|Standard Mode | for Qt5 Only  |(C++ & QML API)|
|:-------------|:--------------|:--------------|
|Compat Mode | for Qt4 & Qt5 |(C++ API Only) |


# What’s new #

  * ExtSerialPort (QextQmlSerialPort)

> Support QML 2

# Getting Started #

> This simplest way to play with this library is opening **qtextserialport.pro** or **compat/qextserialport.pro** with Qt Creator.

## Standard Mode ##

Build and use this library as a module of Qt5

More information can be found: http://wiki.qt-project.org/Creating_a_new_module_or_tool_for_Qt

Note: Perl is needed.

### Build and install ###

At the toplevel directory, run
```
    qmake
    make
```

### C++ API ###

Add following line to your qmake project file:
```
    QT += extserialport
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

### QML API ###
```
    import QtExtSerialPort 2.0

    ExtSerialPort
    {
        id: port
        portName: "COM22"
        baudRate: 19200
        parity: ExtSerialPort.PAR_EVEN
        dataBits: ExtSerialPort.DATA_6
        connected: true
        //shown data in TextEdit
        onDataAvailable: recvText.text = port.stringData
    }
```

## Compat Mode ##

  * Provide the same usage as [QextSerialPort1.2](QextSerialPort_1_2_RC.md)
  * Support Qt4 and Qt5
  * Can be used as static or shared library, or simply integrate the component into application.

### Usage(1)：Source Code Only ###

> The package contains a `qextserialport.pri` file that allows you to integrate the component into programs that use qmake for the build step.

  * Download the source code.

  * Put the source code in any directory you like. For example, 3rdparty:
```
|-- project.pro
|-- ....
|-- 3rdparty\
|     |-- extserialport\
|     |
```

  * Add following line to your qmake project file:
```
include(3rdparty/extserialport/qextserialport.pri)
```

  * Then, using QextSerialPort in your code

**Hint:** If you like, you can copy all files from qextserialport/src to your application's source path. then add following line to your project file
```
include(qextserialport.pri)
```

### Usage(2)：Shared Library ###
> Although QextSerialPort can be directly compiled into your application, you may prefer to use QextSerailPort as a shared library, which is very easy too.

  * Goto the compat  directory , and run
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

**Not familiar with how to use share library?** Perhaps this is useful for you [HowToUseLibrary](HowToUseLibrary.md)

### Usage(3)：Static Library ###
Someone prefer to use QextSerailPort as static library.

  * Open the project file: **compat/qextserialport.pro**, add uncomment follow line
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

# Details #

Any comments are welcome.

And you can discuss with or join us through [mail list](http://groups.google.com/group/qextserialport)