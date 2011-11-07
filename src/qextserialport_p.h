#ifndef _QEXTSERIALPORT_P_H_
#define _QEXTSERIALPORT_P_H_

#include "qextserialport.h"
#ifdef Q_OS_UNIX
#  include <termios.h>
#elif (defined Q_OS_WIN)
/*we should always use <qt_windows.h> instead of <windows.h>*/
#  include <qt_windows.h>
#endif

/*if all warning messages are turned off, flag portability warnings to be turned off as well*/
#ifdef _TTY_NOWARN_
#  define _TTY_NOWARN_PORT_
#endif

/*macros for warning and debug messages*/
#ifdef _TTY_NOWARN_PORT_
#  define TTY_PORTABILITY_WARNING  while(false)qWarning
#else
#  define TTY_PORTABILITY_WARNING qWarning
#endif /*_TTY_NOWARN_PORT_*/

#ifdef _TTY_NOWARN_
#  define TTY_WARNING while(false)qWarning
#else
#  define TTY_WARNING qWarning
#endif /*_TTY_NOWARN_*/

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
