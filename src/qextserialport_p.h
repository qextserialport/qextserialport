/****************************************************************************
** Copyright (c) 2004-2007 Stefan Sander
** Copyright (c) 2007 Michal Policht
** Copyright (c) 2008 Brandon Fosdick
** Copyright (c) 2009-2010 Liam Staskawicz
** Copyright (c) 2011 Debao Zhang
** All right reserved.
** Web: http://code.google.com/p/qextserialport/
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/

#ifndef _QEXTSERIALPORT_P_H_
#define _QEXTSERIALPORT_P_H_

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QESP API.  It exists for the convenience
// of other QESP classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qextserialport.h"
#ifdef Q_OS_UNIX
#  include <termios.h>
#elif (defined Q_OS_WIN)
/*we should always use <qt_windows.h> instead of <windows.h>*/
#  include <qt_windows.h>
#endif

class QWinEventNotifier;
class QReadWriteLock;
class QMutex;
class QSocketNotifier;

class QextSerialPortPrivate
{
    Q_DECLARE_PUBLIC(QextSerialPort)
public:
    QextSerialPortPrivate(QextSerialPort * q);
    ~QextSerialPortPrivate();
    enum DirtyFlagEnum
    {
        DFE_BaudRate = 0x0001,
        DFE_Parity = 0x0002,
        DFE_StopBits = 0x0004,
        DFE_DataBits = 0x0008,
        DFE_Flow = 0x0010,
        DFE_TimeOut = 0x0100,
        DFE_ALL = 0x0fff,
        DFE_Settings_Mask = 0x00ff //without TimeOut
    };
    QMutex* mutex;
    QString port;
    PortSettings Settings;
    int settingsDirtyFlags;
    ulong lastErr;
    QextSerialPort::QueryMode _queryMode;

    // platform specific members
#ifdef Q_OS_UNIX
    int fd;
    QSocketNotifier *readNotifier;
    struct termios Posix_CommConfig;
    struct termios old_termios;
#elif (defined Q_OS_WIN)
    HANDLE Win_Handle;
    OVERLAPPED overlap;
    COMMCONFIG Win_CommConfig;
    COMMTIMEOUTS Win_CommTimeouts;
    QWinEventNotifier *winEventNotifier;
    DWORD eventMask;
    QList<OVERLAPPED*> pendingWrites;
    QReadWriteLock* bytesToWriteLock;
    qint64 _bytesToWrite;
#endif

    /*fill PortSettings*/
    void setBaudRate(BaudRateType baudRate, bool update=true);
    void setDataBits(DataBitsType dataBits, bool update=true);
    void setParity(ParityType parity, bool update=true);
    void setStopBits(StopBitsType stopbits, bool update=true);
    void setFlowControl(FlowType flow, bool update=true);
    void setTimeout(long millisec, bool update=true);
    void setPortSettings(const PortSettings& settings, bool update=true);

    void platformSpecificDestruct();
    void platformSpecificInit();
    void translateError(ulong error);
    void updatePortSettings();

    qint64 readData_sys(char * data, qint64 maxSize);
    qint64 writeData_sys(const char * data, qint64 maxSize);
    void setDtr_sys(bool set=true);
    void setRts_sys(bool set=true);
    bool open_sys(QIODevice::OpenMode mode);
    bool close_sys();
    bool flush_sys();
    ulong lineStatus_sys();
    qint64 bytesAvailable_sys() const;

#ifdef Q_OS_WIN
    void _q_onWinEvent(HANDLE h);
#endif

    QextSerialPort * q_ptr;
};

#endif //_QEXTSERIALPORT_P_H_
