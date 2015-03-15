All the developers of Qt should have been familiar with normal usage of MOC.

## How to use moc(1) ##
  * Consider that we have a Qt project, which contains a subclass of QObject, and the definition of the class is in a header file.

```
// myclass.h
class MyClass : public QObject
{
Q_OBJECT
public:
     MyClass();
};
```

  * When qmake is running, once it find macro Q\_OBJECT in header file, it will call moc for us.

```
moc myclass.h -o moc_myclass.cpp
```

  * As moc\_myclass.cpp is a compile unit, so qmake will run compiler to compile it, then link the moc\_myclass.o(bj) to other part. All of them are transparent for users.
```
g++ moc_myclass.cpp -o moc_myclass.o
```

## How to use moc(2) ##
  * What will happen if the definition of the class located in one .cpp source file?

```
// main.cpp
class MyClass : public QObject
{
Q_OBJECT
public:
     MyClass(){}
};

int main(int argc, char**argv)
{
...
}
```

  * When qmake is running, once it find macro Q\_OBJECT in source file, it will call moc for us too.

```
moc main.cpp -o main.moc
```

  * Then what's the difference?

  * main.moc is not a compile unit any, as it will dependent on the definition of class MyClass.So it can not include the file "main.cpp".
```
# This will fail ....
g++ main.moc -o main_moc.o
```

  * How to solve this?
```
//Add following line to main.cpp file, after the definition of MyClass
#include "main.moc"
```

## How to use moc(3) ##
If you have read the source code of qextserialport, you will find that

  * QextSerialPort defined in the header file qextserialport.h
  * But qextserialport.cpp contains following line
```
#include "moc_qextserialport.cpp"
```

  * So, when qmake is running, once it find macro Q\_OBJECT in header file, it will call moc for us.

```
moc myclass.h -o moc_myclass.cpp
```

  * But, why we include "moc\_qextserialport.cpp" manully?

In order to move private data member and private slots to non-public files, d-pointer and     Q\_PRIVATE\_SLOT are used.

So the generated moc\_qextserialport.cpp is dependent on non-public files will make it can not be compiled as a compile unit.

```
# This doesn't work any more ;-)
g++ moc_qextserialport.cpp -o moc_qextserialport.o
```

## why both moc\_xxx.cpp and xxx.moc exist? ##
In same cases, both myclass.h and myclass.cpp may  have Q\_OBJECT

```
// myclass.h
class MyClass : public QObject
{
Q_OBJECT
public:
     MyClass();
};
```

```
// myclass.cpp
class MyClassHelper : public QObject
{
Q_OBJECT
public:
     MyClassHelper();
};

MyClass::MyClass()
{
}
...
```

In such cases, moc will generated two files for us
```
moc myclass.h -o moc_myclass.cpp
moc myclass.cpp -o myclass.moc
```