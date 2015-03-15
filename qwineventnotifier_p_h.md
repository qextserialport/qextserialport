## ../../../src/corelib/kernel/qwineventnotifier\_p.h: No such file or directory ##

This is a bug of QtSDK installer!

For example,  the contents of file

  * `C:\QtSDK\Desktop\Qt\4.8.0\mingw\include\QtCore\private\qwineventnotifier_p.h`

is

```
 #include "../../../src/corelib/kernel/qwineventnotifier_p.h"
```
. But the path is wrong. The right one should be
```
  #include "c:\QtSDK\QtSources\4.8.0/src/corelib/kernel/qwineventnotifier_p.h"
```
or
```
  #include "../../../../../../../QtSources/4.8.0/src/corelib/kernel/qwineventnotifier_p.h"
```

### workaround ###

Note that, Qt4's QtSDK doesn't supported by Qt-Project any more. you had better download
the latest Qt4.8.X Library and QtCreator seperately.

### Another workaround: ###

download file
```
http://qt.gitorious.org/qt/qt/blobs/4.8/src/corelib/kernel/qwineventnotifier_p.h
```
then replace above broken file with it.

## QextWinEventNotifier ##

This file was provided in QextSerialPort 1.2 beta1 and beta2 as another workaround.

The QextWinEventNotifier class provides support for the Windows Wait functions.

The QextWinEventNotifier class makes it possible to use the wait functions on windows in a asynchronous manner. With this class you can register a HANDLE to an event and get notification when that event becomes signalled.

If it is a manual reset event ,it will be reset before the notification. This is different from QWinEventNotifier.
All the registered handles will be waited under a new thread. This is different from QWinEventNotifier whose event handle will be waited in its affinal thread.