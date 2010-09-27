
#ifndef _QEXTSERIALPORT_H_
#define _QEXTSERIALPORT_H_

#include "qextserialport_global.h"

/*if all warning messages are turned off, flag portability warnings to be turned off as well*/
#ifdef _TTY_NOWARN_
#define _TTY_NOWARN_PORT_
#endif

/*macros for warning and debug messages*/
#ifdef _TTY_NOWARN_PORT_
#define TTY_PORTABILITY_WARNING(s)
#else
#define TTY_PORTABILITY_WARNING(s) qWarning(s)
#endif /*_TTY_NOWARN_PORT_*/
#ifdef _TTY_NOWARN_
#define TTY_WARNING(s)
#else
#define TTY_WARNING(s) qWarning(s)
#endif /*_TTY_NOWARN_*/


/*line status constants*/
#define LS_CTS  0x01
#define LS_DSR  0x02
#define LS_DCD  0x04
#define LS_RI   0x08
#define LS_RTS  0x10
#define LS_DTR  0x20
#define LS_ST   0x40
#define LS_SR   0x80

/*error constants*/
#define E_NO_ERROR                   0
#define E_INVALID_FD                 1
#define E_NO_MEMORY                  2
#define E_CAUGHT_NON_BLOCKED_SIGNAL  3
#define E_PORT_TIMEOUT               4
#define E_INVALID_DEVICE             5
#define E_BREAK_CONDITION            6
#define E_FRAMING_ERROR              7
#define E_IO_ERROR                   8
#define E_BUFFER_OVERRUN             9
#define E_RECEIVE_OVERFLOW          10
#define E_RECEIVE_PARITY_ERROR      11
#define E_TRANSMIT_OVERFLOW         12
#define E_READ_FAILED               13
#define E_WRITE_FAILED              14
#define E_FILE_NOT_FOUND            15

enum BaudRateType
{
    BAUD50,                //POSIX ONLY
    BAUD75,                //POSIX ONLY
    BAUD110,
    BAUD134,               //POSIX ONLY
    BAUD150,               //POSIX ONLY
    BAUD200,               //POSIX ONLY
    BAUD300,
    BAUD600,
    BAUD1200,
    BAUD1800,              //POSIX ONLY
    BAUD2400,
    BAUD4800,
    BAUD9600,
    BAUD14400,             //WINDOWS ONLY
    BAUD19200,
    BAUD38400,
    BAUD56000,             //WINDOWS ONLY
    BAUD57600,
    BAUD76800,             //POSIX ONLY
    BAUD115200,
    BAUD128000,            //WINDOWS ONLY
    BAUD256000             //WINDOWS ONLY
};

enum DataBitsType
{
    DATA_5,
    DATA_6,
    DATA_7,
    DATA_8
};

enum ParityType
{
    PAR_NONE,
    PAR_ODD,
    PAR_EVEN,
    PAR_MARK,               //WINDOWS ONLY
    PAR_SPACE
};

enum StopBitsType
{
    STOP_1,
    STOP_1_5,               //WINDOWS ONLY
    STOP_2
};

enum FlowType
{
    FLOW_OFF,
    FLOW_HARDWARE,
    FLOW_XONXOFF
};

/**
 * structure to contain port settings
 */
struct PortSettings
{
    BaudRateType BaudRate;
    DataBitsType DataBits;
    ParityType Parity;
    StopBitsType StopBits;
    FlowType FlowControl;
    long Timeout_Millisec;
};

#include <QIODevice>
#include <QMutex>
#ifdef Q_OS_UNIX
#include <stdio.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <QSocketNotifier>
#elif (defined Q_OS_WIN)
#include <windows.h>
#include <QThread>
#include <QReadWriteLock>
#include <QtCore/private/qwineventnotifier_p.h>
#endif

class QEXTSERIALPORT_EXPORT QextSerialPort: public QIODevice
{
    Q_OBJECT
public:
    enum QueryMode {
        Polling,
        EventDriven
    };

    QextSerialPort(QueryMode mode = EventDriven);
    QextSerialPort(const QString & name, QueryMode mode = EventDriven);
    QextSerialPort(PortSettings const& s, QueryMode mode = EventDriven);
    QextSerialPort(const QString & name, PortSettings const& s, QueryMode mode = EventDriven);
    ~QextSerialPort();

    void setPortName(const QString & name);
    QString portName() const;

    inline QueryMode queryMode() const { return _queryMode; }
    void setQueryMode(QueryMode mode);

    void setBaudRate(BaudRateType);
    BaudRateType baudRate() const;

    void setDataBits(DataBitsType);
    DataBitsType dataBits() const;

    void setParity(ParityType);
    ParityType parity() const;

    void setStopBits(StopBitsType);
    StopBitsType stopBits() const;

    void setFlowControl(FlowType);
    FlowType flowControl() const;

    void setTimeout(long);

    bool open(OpenMode mode);
    bool isSequential() const;
    void close();
    void flush();

    qint64 size() const;
    qint64 bytesAvailable() const;
    QByteArray readAll();

    void ungetChar(char c);

    ulong lastError() const;
    void translateError(ulong error);

    void setDtr(bool set=true);
    void setRts(bool set=true);
    ulong lineStatus();
    QString errorString();

#ifdef Q_OS_WIN
    virtual bool waitForReadyRead(int msecs);  ///< @todo implement.
    virtual qint64 bytesToWrite() const;
    static QString fullPortNameWin(const QString & name);
#endif

protected:
    QMutex* mutex;
    QString port;
    PortSettings Settings;
    ulong lastErr;
    QueryMode _queryMode;

    // platform specific members
#ifdef Q_OS_UNIX
    int fd;
    QSocketNotifier *readNotifier;
    struct termios Posix_CommConfig;
    struct termios old_termios;
    struct timeval Posix_Timeout;
    struct timeval Posix_Copy_Timeout;
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

    void construct(); // common construction
    void platformSpecificDestruct();
    void platformSpecificInit();
    qint64 readData(char * data, qint64 maxSize);
    qint64 writeData(const char * data, qint64 maxSize);

#ifdef Q_OS_WIN
private slots:
    void onWinEvent(HANDLE h);
#endif

private:
    Q_DISABLE_COPY(QextSerialPort)

signals:
    void dsrChanged(bool status);
};

#endif
