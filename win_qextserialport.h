#ifndef _WIN_QEXTSERIALPORT_H_
#define _WIN_QEXTSERIALPORT_H_

#include "qextserialbase.h"

/*if all warning messages are turned off, flag portability warnings to be turned off as well*/
#ifdef _TTY_NOWARN_
#define _TTY_NOWARN_PORT_
#endif

#include <windows.h>

class Win_QextSerialPort:public QextSerialBase {
public:
    Win_QextSerialPort();
    Win_QextSerialPort(Win_QextSerialPort const& s);
    Win_QextSerialPort(const char* name);
    Win_QextSerialPort(const PortSettings& settings);
    Win_QextSerialPort(const char* name, const PortSettings& settings);
    Win_QextSerialPort& operator=(Win_QextSerialPort const& s);
    virtual ~Win_QextSerialPort();
    virtual bool open(int mode=0);
    virtual void close();
    virtual void flush();
    virtual Offset size() const;
    virtual int getch();
    virtual int putch(int);
    virtual int ungetch(int);
    virtual void setFlowControl(FlowType);
    virtual void setParity(ParityType);
    virtual void setDataBits(DataBitsType);
    virtual void setStopBits(StopBitsType);
    virtual void setBaudRate(BaudRateType);
    virtual void setDtr(bool set=true);
    virtual void setRts(bool set=true);
    virtual unsigned long lineStatus(void);
    virtual int bytesWaiting();
    virtual void translateError(unsigned long);
    virtual void setTimeout(unsigned long=0, unsigned long=0);

#ifdef QTVER_PRE_30
    virtual Q_LONG readBlock(char *data, uint maxlen);
    virtual Q_LONG writeBlock(const char *data, uint len);
#else 
    virtual Q_LONG readBlock(char *data, unsigned long maxlen);
    virtual Q_LONG writeBlock(const char *data, unsigned long len);
#endif

protected:
    HANDLE Win_Handle;
    COMMCONFIG Win_CommConfig;
    COMMTIMEOUTS Win_CommTimeouts;

    void construct(void);
};

#endif
