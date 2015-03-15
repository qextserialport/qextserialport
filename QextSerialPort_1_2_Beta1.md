

# Getting Started #

The package contains a qextserialport.pri file that allows you to integrate the component into programs which use qmake for the build step. Although QextSerialPort can be directly compiled into your application, You may prefer to use QextSerailPort as an library, which is very easy too.

## How to use (1) ##
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

  * Using QextSerialPort in your code. Enjoy it!
```
#include "qextserialport.h"
....
QextSerialPort * port = new QextSerialPort();
....
```

## How to use (2) ##
It's very easy to compile QextSerialPort directly into your application(see above section), however, we would prefer to use it as a static or shared library.

  * Download the source code, and put it in any location you like.
```
|-- yourpath\
|     |-- qextserialport\
|     |
```

  * Create a config.pri file, and put into qextserialport's directory.
```
|-- yourpath\
|     |-- qextserialport\
|     |     |-- config.pri
```
    * Contents of config\_example.pri
```
# uncomment the following line if you want to use qextserialport as library
# QEXTSERIALPORT_LIBRARY = yes

# uncomment the following line too if you want to use it as static library
# QEXTSERIALPORT_STATIC = yes
```

  * Goto qextserialport/buildlib, and run following command to generate library.
```
qmake
make (or nmake)
```

  * Add following line to your qmake project file. Enjoy it!
    * Yes, this line is exactly the same as the previous one. The magic is config.pri
```
include(pathToQextserialport/src/qextserialport.pri)
```

  * Note
    * For Windows user: qextserialport-1.2(d).dll will be automatically copied into your %QTDIR%/bin directory so that it can be found by all your applications.
    * For Unix user: rpath is used when using the libqextserialport-1.2.so file, so that it can be found by your applications. (However, you had better avoid this in real application: http://labs.qt.nokia.com/2011/10/28/rpath-and-runpath/ )
    * Name and location of the library are specified in the common.pri file.

## How to use (3) ##
Someone complains that, I want to used QextSerialPort as a shared libaray, but I don't want to add the `include(***.pri)` to my project file. Otherwise, all the source files of QextSerialPort will be shown in Qt Creator, which is very annoying.

Ok, let's go!

  * Download the source code, and put it in any location you like.
  * Goto qextserialport/buildlib, and run following command to generate library. (Yes, config.pri is not needed.)
```
qmake
make (or nmake)
```
  * Add following lines to your .pro file. And you need to specify the path to the lib and headers. Then Enjoy it!
```
#####
# Specify lib and headers path if they are not in standard locations.
# Otherwise, your can ignore this.
unix{
   QEXTSERIALPORT_LIBDIR = #path_to_qextserialport_lib
   QEXTSERIALPORT_INCDIR = #path_to_qextserialport_headers
}else{
   QEXTSERIALPORT_LIBDIR = #path_to_qextserialport_lib
   QEXTSERIALPORT_INCDIR = #path_to_qextserialport_headers
}
#####

# Generated proper lib name for all platforms. (same as common.pri)
SAVE_TEMPLATE = $$TEMPLATE
TEMPLATE = fakelib
QEXTSERIALPORT_LIBNAME = $$qtLibraryTarget(qextserialport-1.2)
TEMPLATE = $$SAVE_TEMPLATE

# Add preprocesser and link options
INCLUDEPATH += $$QEXTSERIALPORT_INCDIR
!isEmpty(QEXTSERIALPORT_LIBDIR):LIBS += -L$$QEXTSERIALPORT_LIBDIR
LIBS += -l$$QEXTSERIALPORT_LIBNAME
DEFINES += QEXTSERIALPORT_USING_SHARED

# Even shared library not in standard locations, your application still
# can find the shared library at running time.
unix:QMAKE_RPATHDIR += $$QEXTSERIALPORT_LIBDIR
```

  * Oh, this file looks a bit complicated, as we need cross-platform. If you don't care cross-platform. It will be very simple :-) , for example
```
      INCLUDEPATH += /home/xxxx/download/qextserialport/src
      LIBS += -L/home/xxxx/download/qextserialport/lib -lqextsrialport-1.2
      DEFINES += QEXTSERIALPORT_USING_SHARED
```

  * Note:
    * If you do not want to add such lines to each of your projects. This is another solution too.
    * Create a new file called **extserialport.prf** , then add above lines to this new file.
    * Put this .prf file to one of feature paths, such as
      * $QTDIR\mkspecs\features\
    * Add the following line to your .pro file. Enjoy it!
```
CONFIG += extserialport
```

## Using Documents ##

### Offline ###

> If you are using qextserialport-XXX.tar.gz, the qesp.qch and
> html files have been provided for you.

> Open the file **html/index.html** using your web browser.

> Or integrated the **html/qesp.qch** into your Qt Creator.

  * For Qt Assistant:
```
Edit ==> Preferences ==>  Documentations ==> Add...
```

  * For Qt Creator
```
Tools ==> Options ==> Help ==> Documentations ==> Add...
```

### Online ###
  * [QextSerialPort 1.2 Manual](http://docs.qextserialport.googlecode.com/hg/1.2/qextserialport.html)

### How to generate help files? ###

Simply run following commands at toplevel directory.
```
    qmake
    make docs
```

Or run the following command at this directory
```
    qdoc3 qextserialport.qdocconf
```

Then a folder called "html" will be generated.

Once the html files are generated. run following commands
```
   cd doc/html
   qhelpgenerator qesp.qhp
```
A file called "qesp.qch" will be generated.

## Build Examples ##
> If your has create a config.pri file properly, an shared or static library will be auto generated and then used by all the examples. Otherwise, qextserialport will be directly compiled into the examples.

  * Method1: Run qmake from the toplevel directory.
```
qmake
make
```

  * Method2: If you have installed Qt Creator, you can simply open the qextserialport.pro file with it.

  * Method3: If you are using Visual Studio under Windows, run
```
qmake -tp vc -r
```
Then open the qextserialport.sln file. Enjoy it!

# Implementation Details #

There are some implementation details which may be useful for developers and contributors.

## QextWinEventNotifier ##

> Some users complain that they can not find QWinEventNotifier when using Qt SDK, so this class was provided as a workaround. see [issue 91](https://code.google.com/p/qextserialport/issues/detail?id=91) , [issue 105](https://code.google.com/p/qextserialport/issues/detail?id=105) and [issue 106](https://code.google.com/p/qextserialport/issues/detail?id=106)

The QextWinEventNotifier class provides support for the Windows Wait functions.

The QextWinEventNotifier class makes it possible to use the wait
functions on windows in a asynchronous manner. With this class
you can register a HANDLE to an event and get notification when
that event becomes signalled.

  * If it is a manual reset event ,it will be reset before the notification. This is different from QWinEventNotifier.
  * All the registered handles will be waited under a new thread. This is different from QWinEventNotifier whose event handle will be waited in its affinal thread.

**Note:** QWinEventNotifier has been a public class under Qt5. http://codereview.qt-project.org/8724

## QextReadBuffer ##

This is QextSerialPort's read buffer, needed by posix system. see [issue 33](https://code.google.com/p/qextserialport/issues/detail?id=33) and [issue 74](https://code.google.com/p/qextserialport/issues/detail?id=74)

## why d-pointer? ##

If we want to changes to the implementation of qextserialport without breaking binary compatibilit, d-pointer is very important. Otherwise application link to qextserialport-1.2 will not work without recompiled when qextserialport updated to 1.3 .

More information can be found from http://qt-project.org/wiki/Dpointer

## qdoc3 instead of doxygen ##
From QextSerialPort-1.2beta1, qdoc3 instead of doxygen is used to generate document files.

qdoc3 is a tool used by Qt Developers to generate documentation for software projects. It works by extracting qdoc comments from project source files and then formatting these comments as HTML pages.

Manual of qdoc3 can be found from here: http://doc.qt.nokia.com/qdoc/

**Note:** qdoc3 has been renamed to qdoc under Qt5.

## Why MIT? ##
From qextserialport-1.2beta1 on, license of the project changed to MIT. More information can be found form [QextSerialPort LICENSE](License.md)