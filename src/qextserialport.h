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
    BAUD134 = 134,              //POSIX ONLY
    BAUD150 = 150,              //POSIX ONLY
    BAUD200 = 200,              //POSIX ONLY
    BAUD1800 = 1800,            //POSIX ONLY
#  ifdef B76800
    BAUD76800 = 76800,          //POSIX ONLY
#  endif
#elif defined(Q_OS_WIN)
    BAUD14400 = 14400,          //WINDOWS ONLY
    BAUD56000 = 56000,          //WINDOWS ONLY
    BAUD128000 = 12800,         //WINDOWS ONLY
    BAUD256000 = 25600,         //WINDOWS ONLY
#endif //Q_OS_UNIX
    BAUD110 = 110,
    BAUD300 = 300,
    BAUD600 = 600,
    BAUD1200 = 1200,
    BAUD2400 = 2400,
    BAUD4800 = 4800,
    BAUD9600 = 9600,
    BAUD19200 = 19200,
    BAUD38400 = 38400,
    BAUD57600 = 57600,
    BAUD115200 = 115200
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

    QextSerialPort(QueryMode mode = EventDriven, QObject* parent = 0);
    QextSerialPort(const QString & name, QueryMode mode = EventDriven, QObject * parent = 0);
    QextSerialPort(PortSettings const& s, QueryMode mode = EventDriven, QObject * parent = 0);
    QextSerialPort(const QString & name, PortSettings const& s, QueryMode mode = EventDriven, QObject *parent=0);

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

Q_SIGNALS:
    void dsrChanged(bool status);

protected:
    qint64 readData(char * data, qint64 maxSize);
    qint64 writeData(const char * data, qint64 maxSize);

private:
    Q_DISABLE_COPY(QextSerialPort)

#ifdef Q_OS_WIN
    Q_PRIVATE_SLOT(d_func(), void _q_onWinEvent(HANDLE))
#endif

    QextSerialPortPrivate * d_ptr;

};

#endif
