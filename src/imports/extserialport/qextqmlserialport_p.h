/****************************************************************************
** Copyright (c) 2012 Debao Zhang
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

#ifndef QEXTQMLSERIALPORT_P_H
#define QEXTQMLSERIALPORT_P_H

#include <QtCore/QObject>
#include <QtQml/qqml.h>

class QextQmlSerialPortPrivate;

class QextQmlSerialPort : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_ENUMS(DataBitsType)
    Q_ENUMS(ParityType)
    Q_ENUMS(StopBitsType)
    Q_ENUMS(FlowType)
    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged)
    Q_PROPERTY(QString stringData READ stringData WRITE sendStringData NOTIFY dataAvailable)
    Q_PROPERTY(QString stringCodec READ stringCodec WRITE setStringCodec)
    Q_PROPERTY(QString portName READ portName WRITE setPortName)
    Q_PROPERTY(int baudRate READ baudRate WRITE setBaudRate)
    Q_PROPERTY(DataBitsType dataBits READ dataBits WRITE setDataBits)
    Q_PROPERTY(ParityType parity READ parity WRITE setParity)
    Q_PROPERTY(StopBitsType stopBits READ stopBits WRITE setStopBits)
    Q_PROPERTY(FlowType flowControl READ flowControl WRITE setFlowControl)

public:

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
#if defined(Q_OS_WIN) || defined(qdoc)
        PAR_MARK,
#endif
        PAR_SPACE
    };

    enum StopBitsType
    {
        STOP_1,
#if defined(Q_OS_WIN) || defined(qdoc)
        STOP_1_5,
#endif
        STOP_2
    };

    enum FlowType
    {
        FLOW_OFF,
        FLOW_HARDWARE,
        FLOW_XONXOFF
    };

    explicit QextQmlSerialPort(QObject *parent = 0);
    ~QextQmlSerialPort();
    
    // From QQmlParserStatus
    void classBegin() {}
    void componentComplete();

    bool connected();
    QString stringData();
    QString stringCodec();
    QString portName();
    int baudRate();
    DataBitsType dataBits();
    ParityType parity();
    StopBitsType stopBits();
    FlowType flowControl();

Q_SIGNALS:
    void connectedChanged();
    void dataAvailable();

public Q_SLOTS:
    void setConnected(bool connect);
    void sendStringData(QString data);
    void setStringCodec(QString codec);
    void setPortName(QString portName);
    void setBaudRate(int baudrate);
    void setDataBits(DataBitsType databits);
    void setParity(ParityType parity);
    void setStopBits(StopBitsType stopbits);
    void setFlowControl(FlowType flow);

private:
    Q_DECLARE_PRIVATE(QextQmlSerialPort)
    QextQmlSerialPortPrivate * const d_ptr;
};

QML_DECLARE_TYPE(QextQmlSerialPort)

#endif // QEXTQMLSERIALPORT_P_H
