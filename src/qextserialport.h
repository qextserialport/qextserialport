/****************************************************************************
** Copyright (c) 2000-2007 Stefan Sander
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

#ifndef _QEXTSERIALPORT_H_
#define _QEXTSERIALPORT_H_

#include <QtCore/QIODevice>
#include "qextserialport_global.h"
#include "qextportsettings.h"

class QextSerialPortPrivate;
class QEXTSERIALPORT_EXPORT QextSerialPort: public QIODevice
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QextSerialPort)
    Q_ENUMS(QueryMode)
    Q_PROPERTY(QString portName READ portName WRITE setPortName)
    Q_PROPERTY(QueryMode queryMode READ queryMode WRITE setQueryMode)
public:
    enum QueryMode {
        Polling,
        EventDriven
    };

    explicit QextSerialPort(QueryMode mode = EventDriven, QObject* parent = 0);
    explicit QextSerialPort(const QString & name, QueryMode mode = EventDriven, QObject * parent = 0);
    explicit QextSerialPort(QextPortSettings const& s, QueryMode mode = EventDriven, QObject * parent = 0);
    QextSerialPort(const QString & name, QextPortSettings const& s, QueryMode mode = EventDriven, QObject *parent=0);

    ~QextSerialPort();

    QString portName() const;
    QueryMode queryMode() const;
    BaudRateType baudRate() const;
    DataBitsType dataBits() const;
    ParityType parity() const;
    StopBitsType stopBits() const;
    FlowType flowControl() const;
    int customBaudRate() const;

    bool open(OpenMode mode);
    bool isSequential() const;
    void close();
    void flush();
    qint64 bytesAvailable() const;
    QByteArray readAll();

    ulong lastError() const;

    ulong lineStatus();
    QString errorString();

public Q_SLOTS:
    void setPortName(const QString & name);
    void setQueryMode(QueryMode mode);
    void setBaudRate(BaudRateType);
    void setDataBits(DataBitsType);
    void setParity(ParityType);
    void setStopBits(StopBitsType);
    void setFlowControl(FlowType);
    void setTimeout(long);
    void setCustomBaudRate(int baudRate);

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
    Q_PRIVATE_SLOT(d_func(), void _q_canRead())

    QextSerialPortPrivate * const d_ptr;
};

#endif
