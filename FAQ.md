

## Relationship with qextserialport.sf.net? ##

As of May 2009, this is the new location of the project. The sourceforge project is no longer active .

## Can I use qextserialport in a commercial product? ##
Yes, from QextSerialPort 1.2-beta1 on, license of the project has been changed to MIT .

More information can be found in [License](License.md)

## ../../../src/corelib/kernel/qwineventnotifier\_p.h: No such file or directory ##

**This is a bug of QtSDK installer!**

As the contents of `QTDIR\include\QtCore\private\qwineventnotifier_p.h` is broken.

Workaround:

download file
```
http://qt.gitorious.org/qt/qt/blobs/4.8/src/corelib/kernel/qwineventnotifier_p.h
```
then replace above broken file with it.

More information can be found in [qwineventnotifier\_p\_h](qwineventnotifier_p_h.md)

## Why include "moc\_qextserialport.cpp" in "qextserialport.cpp"? ##
In order to move private data member and private slots to non-public files, d-pointer and Q\_PRIVATE\_SLOT are used.
So the generated moc\_qextserialport.cpp is dependent on non-public files, which make it can not be compiled as a compile unit any more.

More information can be found in [Three\_Usage\_Of\_MOC](Three_Usage_Of_MOC.md)

## Is there any other serial port library exists for Qt? ##

Yes, Some of them can be found in [OtherSerialPortLibraries](OtherSerialPortLibraries.md)