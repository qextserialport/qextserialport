#ifndef _QEXTSERIALBASE_H_
#define _QEXTSERIALBASE_H_

#include <qobject.h>
#include <qiodevice.h>
#include <qfile.h>

#ifdef QT_THREAD_SUPPORT
#include <qthread.h>
#endif

/*if all warning messages are turned off, flag portability warnings to be turned off as well*/
#ifdef _TTY_NOWARN_
#define _TTY_NOWARN_PORT_
#endif

/*QT3 changed some return types in QIODevice - these typedefs will retain compatibility with 
  earlier versions*/
#ifdef QTVER_PRE_30
typedef uint Offset;
typedef int Q_LONG;
#else

/*Some compilers (VC++) don't inherit this typedef from QIODevice.h - copied here*/
#ifdef _MSC_VER
#ifdef QT_LARGE_FILE_SUPPORT
    typedef off_t Offset;
#else
    typedef Q_ULONG Offset;
#endif //_MSC_VER
#endif //QT_LARGE_FILE_SUPPORT
#endif //QTVER_PRE_30

/*macros for thread support*/
#ifdef QT_THREAD_SUPPORT
#define LOCK_MUTEX() mutex->lock()
#define UNLOCK_MUTEX() mutex->unlock()
#else
#define LOCK_MUTEX() 
#define UNLOCK_MUTEX() 
#endif

/*macros for warning messages*/
#ifdef _TTY_NOWARN_PORT_
#define TTY_PORTABILITY_WARNING(s) 
#else
#define TTY_PORTABILITY_WARNING(s) qWarning(s)
#endif
#ifdef _TTY_NOWARN_
#define TTY_WARNING(s)
#else
#define TTY_WARNING(s) qWarning(s)
#endif


/*simple MIN macro - evaluates to the smaller of the 2 members*/
#define MIN(a,b) (((a)<(b))?(a):(b))

/*limit of length of port name, not including NULL terminator*/
#define PORT_NAME_SIZE_LIMIT 80

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

/*enums for port settings*/
typedef enum _NamingConvention {
    WIN_NAMES,
    IRIX_NAMES,
    HPUX_NAMES,
    SUN_NAMES,
    LINUX_NAMES,
    DIGITAL_NAMES
} NamingConvention;

typedef enum _FlowType {
    FLOW_OFF, 
    FLOW_HARDWARE, 
    FLOW_XONXOFF
} FlowType;

typedef enum _ParityType {
    PAR_NONE, 
    PAR_ODD,
    PAR_EVEN, 
    PAR_MARK,               //WINDOWS ONLY
    PAR_SPACE
} ParityType;

typedef enum _DataBitsType {
    DATA_5,
    DATA_6,
    DATA_7,
    DATA_8
} DataBitsType;

typedef enum _StopBitsType {
    STOP_1, 
    STOP_1_5,               //WINDOWS ONLY
    STOP_2
} StopBitsType;

typedef enum _BaudRateType {
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
} BaudRateType; 

/*structure to contain port settings*/
typedef struct _PortSettings {
    FlowType FlowControl;
    ParityType Parity;
    DataBitsType DataBits;
    StopBitsType StopBits;
    BaudRateType BaudRate;
    unsigned long Timeout_Sec;
    unsigned long Timeout_Millisec;
} PortSettings;

class QextSerialBase:public QIODevice {
public:
    QextSerialBase();
    QextSerialBase(const char* name);
    virtual ~QextSerialBase();
    virtual void construct(void);
    virtual const char* name() const;
    virtual void setName(const char* name);
    virtual bool open(int mode=0)=0;
    virtual bool open(const char* name);
    virtual void close()=0;
    virtual void flush()=0;
    virtual Offset size() const=0;
    virtual int readLine(char *data, uint maxlen);
    virtual int getch()=0;
    virtual int putch(int)=0;
    virtual int ungetch(int);
    virtual bool atEnd() const;
    virtual void setFlowControl(FlowType)=0;
    virtual FlowType flowControl() const;
    virtual void setParity(ParityType)=0;
    virtual ParityType parity() const;
    virtual void setDataBits(DataBitsType)=0;
    virtual DataBitsType dataBits() const;
    virtual void setStopBits(StopBitsType)=0;
    virtual StopBitsType stopBits() const;
    virtual void setBaudRate(BaudRateType)=0;
    virtual BaudRateType baudRate() const;
    virtual bool isOpen() const;
    virtual unsigned long lastError() const;
    virtual void setDtr(bool set=true)=0;
    virtual void setRts(bool set=true)=0;
    virtual unsigned long lineStatus(void)=0;
    virtual int bytesWaiting()=0;
    virtual void translateError(unsigned long)=0;
    virtual void setTimeout(unsigned long, unsigned long)=0;
    virtual bool isOpen(void);

#ifdef QTVER_PRE_30
    virtual Q_LONG readBlock(char *data, uint maxlen)=0;
    virtual Q_LONG writeBlock(const char *data, uint len)=0;
#else
    virtual Q_LONG readBlock(char *data, unsigned long maxlen)=0;
    virtual Q_LONG writeBlock(const char *data, unsigned long len)=0;
#endif

protected:
    bool portOpen;
    unsigned long lastErr;
    char portName[PORT_NAME_SIZE_LIMIT+1];
    PortSettings Settings;

#ifdef QT_THREAD_SUPPORT
    static unsigned long refCount;
    static QMutex* mutex;
#endif
};

#endif
