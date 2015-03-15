

When we use a shared library, we need to solve too problems:
  * How to locate Libraries at Link Time
  * How to locate Libraries at Runtime

# Overview #
  * Consider that we have a very simply Qt program:
```
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    qDebug()<<"hello qt!";
    app.exec();
}
```

## Then how can we compile it? ##
  * We need to tell the compiler where to find the header files.
    * In this example, compiler need to know in which path the header file `QtCore/QCoreApplication` can be found
  * We need to tell the linker which is the library we needed.
    * In this example, we should tell linker that QtCore4.lib(or libQtCore4.a or libQtCore.so.4) is needed by the application.

## build(1) ##
  * for gcc user:
```
g++ main.cpp -Ie:\Qt\4.8.0\include -o main -Le:\Qt\4.8.0\lib -lQtCore4
```

  * for msvc user
```
cl main.cpp -ID:/Qt/4.8.0/include -Femain -link -LIBPATH:D:/Qt/4.8.0/lib QtCore4.lib
```

## build(2) ##
If we use building tools instead of calling compiler directly, we can not pass these options to compiler directly any more, but all the building tools will provide some way for you to setting such options.

So, for qmake user, .pro file is need.

```
# example.pro

CONFIG -= qt
INCLUDEPATH += e:\Qt\4.8.0\include
LIBS += -Le:\Qt\4.8.0\lib -lQtCore4

SOURCES += main.cpp
```

Then all your need is
```
qmake
make
```

## build(3) ##
Setting INCLUDEPATH and LIBS manually is not easy, especially when you expect this file can be used in a cross-platform way.

The good news is that, we need not to do this for Qt's build-in modules.

So, all we need to do is
```
#example.pro

CONFIG += qt   #default value, can be omitted
QT += core     #default value, can be omitted

SOURCES += main.cpp
```

Then run following commands
```
qmake
make
```

### How does this work? ###
  * When qmake find that **qt** is contained in the **CONFIG** variable, it will try to load a feature file which called **qt.prf** ($QTDIR/mkspec/features/)

  * Codes in this file will try to scan another variable **QT** , when it find **core**, it will set INCLUDEPATH and LIBS for the module according to the platform you used and location Qt installed.

## Runing Application ##

After the application is sucessfully built, we still need to tell the application where to find the shared library it needed.

For Windows User, the shared library should locate in one of following paths:
  * The directory from which the application loaded.
  * The system directory. Use the GetSystemDirectory function to get the path of this directory.
  * The 16-bit system directory. There is no function that obtains the path of this directory, but it is searched.
  * The Windows directory. Use the GetWindowsDirectory function to get the path of this directory.
  * The current directory.
  * The directories that are listed in the PATH environment variable.


For Unix User,
  * Directory specified by `RPATH` when building the application
  * Each of the directories listed in the colon-separated list in the environment variable `LD_LIBRARY_PATH`.
  * The list of libraries found in the file `/etc/ld.so.cache`,which is maintained by the `ldconfig` utility.
  * Directory specified by `RUNPATH` when building the application
  * The directory `/lib`.
  * The directory `/usr/lib`.

# General Library #

Once you successfully build the QextSerialPort shared library, you will get

  * `libqextserialport.so.1.2.0`
  * `libqextserialport{_debug}.1.2.0.dylib`
  * `libqextserialport{d}1.a`
  * `qextserialport{d}1.lib`
  * `qextserialport{d}1.dll`
  * `...`

## How to use?(1) ##
  * Normally, you need to add three lines to your project file

```
INCLUDEPATH += /home/xxxx/download/qextserialport/src
LIBS += /home/xxxx/download/qextserialport/lib/libqextsrialport.so.1.2.0
DEFINES += QEXTSERIALPORT_USING_SHARED
```

  * But above lines only works under one platform, and it will be very complex if you want to get cross-platform solution.

```
win32{
    INCLUDEPATH += 
    CONFIG(debug, debug|release) {
        LIBS +=
    } else {
        LIBS +=
    }
}

unix:!mac:!linux*{
    INCLUDEPATH +=
    LIBS += 
} else:mac {
    INCLUDEPATH +=
    LIBS += 
} else {
    INCLUDEPATH +=
    LIBS += 
}

# ...

DEFINES += QEXTSERIALPORT_USING_SHARED
```

  * Very annoying, isn't it?

## How to use?(2) ##
Instead of add above lines to every project which is dependent on this library.

We can put them to
  * extserialport.pri
Or
  * extserialport.prf

Then include this file in our XXXX.pro file.

For XXX.pri:
```
include(XXX.pri)
```

For XXX.prf
```
CONFIG += XXX
```