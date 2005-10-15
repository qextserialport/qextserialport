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
    Win_QextSerialPort(const QString & name);
    Win_QextSerialPort(const PortSettings& settings);
    Win_QextSerialPort(const QString & name, const PortSettings& settings);
    Win_QextSerialPort& operator=(Win_QextSerialPort const& s);
    virtual ~Win_QextSerialPort();
    virtual bool open(int mode=0);
    virtual void close();
    virtual void flush();
    virtual qint64 size() const;
    virtual bool getChar(char * c);
    virtual bool putChar(char c);
    virtual void ungetChar(char c);
    virtual void setFlowControl(FlowType);
    virtual void setParity(ParityType);
    virtual void setDataBits(DataBitsType);
    virtual void setStopBits(StopBitsType);
    virtual void setBaudRate(BaudRateType);
    virtual void setDtr(bool set=true);
    virtual void setRts(bool set=true);
    virtual unsigned long lineStatus(void);
    virtual qint64 bytesAvailable();
    virtual void translateError(unsigned long);
    virtual void setTimeout(unsigned long=0, unsigned long=0);
    virtual qint64 readData(char *data, qint64 maxSize);
    virtual qint64 writeData(const char *data, qint64 maxSize);

protected:
    HANDLE Win_Handle;
    COMMCONFIG Win_CommConfig;
    COMMTIMEOUTS Win_CommTimeouts;
};

#endif
