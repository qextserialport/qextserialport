

# <font color='red'>Known Issue</font> #

  * [Issue 154](https://code.google.com/p/qextserialport/issues/detail?id=154) : Compile error under MinGW.

Please open the file qextserialenumerator\_p.h, replace the 0x0500 with 0x0501, then it will work. Meaning of WINVER can be found in [MinGW\_and\_WINVER](MinGW_and_WINVER.md)

  * QtSDK of Qt4: ../../../src/corelib/kernel/qwineventnotifier\_p.h: No such file or directory

This is a bug of Qt4's QTSDK. Workaround can be found in [qwineventnotifier\_p\_h](qwineventnotifier_p_h.md)

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

## Why remove QextWinEventNotifier? ##
This class was added in [QextSerialPort\_1\_2\_Beta1](QextSerialPort_1_2_Beta1.md) as

```
Some users complain that they can not find QWinEventNotifier when using Qt SDK, 
so this class was provided as a workaround. see  issue 91  ,  issue 105  and  issue 106 
```

But it turnout to be a bug of QtSDK installer. And many workaround can be used for QtSDK user.

More information can be found in [qwineventnotifier\_p\_h](qwineventnotifier_p_h.md)

**Note**: Windows user who use Qt Creator+Qt Library instead of QtSDK will free of this problem too.