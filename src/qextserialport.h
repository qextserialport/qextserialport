#ifndef _QEXTSERIALPORT_H_
#define _QEXTSERIALPORT_H_

#include <QtCore/QIODevice>
#include "qextserialport_global.h"

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
#ifdef Q_OS_UNIX
    BAUD50 = 50,                //POSIX ONLY
    BAUD75 = 75,                //POSIX ONLY
#endif
    BAUD110 = 110,
#ifdef Q_OS_UNIX
    BAUD134 = 134,               //POSIX ONLY
    BAUD150 = 150,               //POSIX ONLY
    BAUD200 = 200,               //POSIX ONLY
#endif
    BAUD300 = 300,
    BAUD600 = 600,
    BAUD1200 = 1200,
#ifdef Q_OS_UNIX
    BAUD1800 = 1800,              //POSIX ONLY
#endif
    BAUD2400 = 2400,
    BAUD4800 = 4800,
    BAUD9600 = 9600,
#ifdef Q_OS_WIN
    BAUD14400 = 14400,             //WINDOWS ONLY
#endif
    BAUD19200 = 19200,
    BAUD38400 = 38400,
#ifdef Q_OS_WIN
    BAUD56000 = 56000,             //WINDOWS ONLY
#endif
    BAUD57600 = 57600,
#if defined(Q_OS_UNIX) && defined(B76800)
    BAUD76800 = 76800,             //POSIX ONLY
#endif
    BAUD115200 = 115200
#ifdef Q_OS_WIN
    ,
    BAUD128000 = 12800,            //WINDOWS ONLY
    BAUD256000 = 25600             //WINDOWS ONLY
#endif
};

enum DataBitsType
{
    DATA_5 = 5,
    DATA_6 = 6,
    DATA_7 = 7,
    DATA_8 = 8
};

enum ParityType
{
    PAR_NONE,
    PAR_ODD,
    PAR_EVEN,
#ifdef Q_OS_WIN
    PAR_MARK,               //WINDOWS ONLY
#endif
    PAR_SPACE
};

enum StopBitsType
{
    STOP_1,
#ifdef Q_OS_WIN
    STOP_1_5,               //WINDOWS ONLY
#endif
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

class QextSerialPortPrivate;
class QEXTSERIALPORT_EXPORT QextSerialPort: public QIODevice
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QextSerialPort)
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

    QString portName() const;
    QueryMode queryMode() const;
    BaudRateType baudRate() const;
    DataBitsType dataBits() const;
    ParityType parity() const;
    StopBitsType stopBits() const;
    FlowType flowControl() const;

    bool open(OpenMode mode);
    bool isSequential() const;
    void close();
    void flush();
    qint64 bytesAvailable() const;
    QByteArray readAll();

    ulong lastError() const;

    ulong lineStatus();
    QString errorString();

#ifdef Q_OS_WIN
    static QString fullPortNameWin(const QString & name);
#endif

public Q_SLOTS:
    void setPortName(const QString & name);
    void setQueryMode(QueryMode mode);
    void setBaudRate(BaudRateType);
    void setDataBits(DataBitsType);
    void setParity(ParityType);
    void setStopBits(StopBitsType);
    void setFlowControl(FlowType);
    void setTimeout(long);

    void setDtr(bool set=true);
    void setRts(bool set=true);

protected:
    qint64 readData(char * data, qint64 maxSize);
    qint64 writeData(const char * data, qint64 maxSize);

private:
    Q_DISABLE_COPY(QextSerialPort)

#ifdef Q_OS_WIN
    Q_PRIVATE_SLOT(d_func(), void _q_onWinEvent(HANDLE))
#endif

    QextSerialPortPrivate * d_ptr;

signals:
    void dsrChanged(bool status);
};

#endif
