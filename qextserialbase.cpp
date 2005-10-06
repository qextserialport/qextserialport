#include "qextserialbase.h"
#include <qthread.h>

/*!
\class QextSerialBase
\version 0.70 (pre-alpha)
\author Wayne Roth

A common base class for Win_QextSerialBase, Posix_QextSerialBase and QextSerialPort.
*/
#ifdef QT_THREAD_SUPPORT
QMutex* QextSerialBase::mutex=NULL;
unsigned long QextSerialBase::refCount=0;
#endif

/*!
\fn QextSerialBase::QextSerialBase()
Default constructor.
*/
QextSerialBase::QextSerialBase():QIODevice() {
    char nameStr[50];

#ifdef _TTY_WIN_
    strcpy(nameStr, "COM1");

#elif defined(_TTY_IRIX_)
    strcpy(nameStr, "/dev/ttyf1");

#elif defined(_TTY_HPUX_)
    strcpy(nameStr, "/dev/tty1p0");

#elif defined(_TTY_SUN_)
    strcpy(nameStr, "/dev/ttya");

#elif defined(_TTY_DIGITAL_)
    strcpy(nameStr, "/dev/tty01");

#elif defined(_TTY_FREEBSD_)
    strcpy(nameStr, "/dev/ttyd1");

#else
    strcpy(nameStr, "/dev/ttyS0");
#endif

    setName(nameStr);
    construct();
}

/*!
\fn QextSerialBase::QextSerialBase(const char* name)
Construct a port and assign it to the device specified by the name parameter.
*/
QextSerialBase::QextSerialBase(const char* name):QIODevice() {
    setName(name);
    construct();
}

/*!
\fn QextSerialBase::~QextSerialBase()
Standard destructor.
*/
QextSerialBase::~QextSerialBase() {

#ifdef QT_THREAD_SUPPORT
    refCount--;
    if (mutex && refCount==0) {
        delete mutex;
        mutex=NULL;
    }
#endif

}

/*!
\fn void QextSerialBase::setName(const char* name)
Sets the name of the device associated with the object, e.g. "COM1", or "/dev/ttyS0".
*/
void QextSerialBase::setName(const char* name) {
    memcpy(portName, name, MIN(strlen(name)+1, PORT_NAME_SIZE_LIMIT));
}

/*!
\fn bool QextSerialBase::atEnd() const
This function will return true if the input buffer is empty (or on error), and false otherwise.
Call QextSerialBase::lastError() for error information.
*/
bool QextSerialBase::atEnd() const {
    if (size()) {
        return true;
    }
    return false;
}

/*!
\fn int QextSerialBase::readLine(char *data, uint maxlen)
This function will read a line of buffered input from the port, stopping when either maxlen bytes
have been read, the port has no more data available, or a newline is encountered.  The value
returned is the length of the string that was read.

  Thanks to Olivier Tubach for the original version of this function.
*/
int QextSerialBase::readLine(char *data, uint maxlen) {
    int bytesAvail=(int)size();
    char* pCur=data;

    /*if nothing waiting, return 0 length*/
    if (bytesAvail<1) {
        return 0;
    }

    /*read a byte at a time for MIN(bytesAvail, maxlen) iterations, or until a newline*/
    while (pCur<(data+bytesAvail) && --maxlen) {
        readBlock(pCur, 1);
        if (*pCur++ == '\n') {
            break;
        }
    }
    *pCur++='\0';

    /*return size of data read*/
    return (pCur-data);
}

/*!
\fn int QextSerialBase::ungetch(int)
This function is included to implement the full QIODevice interface, and currently has no
purpose within this class.  This function is meaningless on an unbuffered device and currently
only prints a warning message to that effect.
*/
int QextSerialBase::ungetch(int) {

    /*meaningless on unbuffered sequential device - return error and print a warning*/
    TTY_WARNING("QextSerialPort: ungetch() called on an unbuffered sequential device - operation is meaningless");
    return -1;
}

/*!
\fn FlowType QextSerialBase::flowControl() const
Returns the type of flow control used by the port.  For a list of possible values returned
by this function, see the definition of the enum FlowType.
*/
FlowType QextSerialBase::flowControl() const {
    return Settings.FlowControl;
}

/*!
\fn ParityType QextSerialBase::parity() const
Returns the type of parity used by the port.  For a list of possible values returned by
this function, see the definition of the enum ParityType.
*/
ParityType QextSerialBase::parity() const {
    return Settings.Parity;
}

/*!
\fn DataBitsType QextSerialBase::dataBits() const
Returns the number of data bits used by the port.  For a list of possible values returned by
this function, see the definition of the enum DataBitsType.
*/
DataBitsType QextSerialBase::dataBits() const {
    return Settings.DataBits;
}

/*!
\fn StopBitsType QextSerialBase::stopBits() const
Returns the number of stop bits used by the port.  For a list of possible return values, see
the definition of the enum StopBitsType.
*/
StopBitsType QextSerialBase::stopBits() const {
    return Settings.StopBits;
}

/*!
\fn BaudRateType QextSerialBase::baudRate(void) const
Returns the baud rate of the serial port.  For a list of possible return values see
the definition of the enum BaudRateType.
*/
BaudRateType QextSerialBase::baudRate(void) const {
    return Settings.BaudRate;
}

/*!
\fn bool QextSerialBase::isOpen(void) const
Returns true if the port associated with the class is currently open, or false if it is not.
*/
bool QextSerialBase::isOpen(void) const {
    return portOpen;
}

/*!
\fn bool QextSerialBase::open(const char* name)
Opens a serial port by name.  The string passed in the name parameter is associated with the
object and subsequent calls to open() may use the no-parameter version.  This function has no
effect if the port associated with the class is already open.  The port is also configured to the
current settings, as stored in the Settings structure.
*/
bool QextSerialBase::open(const char* name) {
    LOCK_MUTEX();
    if (!portOpen) {

        /*set serial port name, and call open()*/
        setName(name);
        UNLOCK_MUTEX();
        return open();
    }
    UNLOCK_MUTEX();
    return portOpen;
}

/*!
\fn unsigned long QextSerialBase::lastError() const
Returns the code for the last error encountered by the port, or E_NO_ERROR if the last port
operation was successful.  Possible error codes are:

\verbatim
Error                           Explanation
---------------------------     -------------------------------------------------------------
E_NO_ERROR                      No Error has occured
E_INVALID_FD                    Invalid file descriptor (port was not opened correctly)
E_NO_MEMORY                     Unable to allocate memory tables (POSIX)
E_CAUGHT_NON_BLOCKED_SIGNAL     Caught a non-blocked signal (POSIX)
E_PORT_TIMEOUT                  Operation timed out (POSIX)
E_INVALID_DEVICE                The file opened by the port is not a character device (POSIX)
E_BREAK_CONDITION               The port detected a break condition
E_FRAMING_ERROR                 The port detected a framing error
                                (usually caused by incorrect baud rate settings)
E_IO_ERROR                      There was an I/O error while communicating with the port
E_BUFFER_OVERRUN                Character buffer overrun
E_RECEIVE_OVERFLOW              Receive buffer overflow
E_RECEIVE_PARITY_ERROR          The port detected a parity error in the received data
E_TRANSMIT_OVERFLOW             Transmit buffer overflow
E_READ_FAILED                   General read operation failure
E_WRITE_FAILED                  General write operation failure
\endverbatim
*/
unsigned long QextSerialBase::lastError() const {
    return lastErr;
}

/*!
\fn const char* QextSerialBase::name() const
Returns a pointer to a string containing the name of the device associated with the object
*/
const char* QextSerialBase::name() const {
    return portName;
}

/*!
\fn void QextSerialBase::construct(void)
Common constructor function, called by all versions of Win_QextSerialPort::Win_QextSerialPort().
Sets up default port settings (115200 8N1 Hardware flow control where supported, otherwise no
flow control, and 500 ms timeout).
*/
void QextSerialBase::construct(void) {

    Settings.BaudRate=BAUD115200;
    Settings.DataBits=DATA_8;
    Settings.StopBits=STOP_1;
    Settings.Parity=PAR_NONE;
    Settings.FlowControl=FLOW_HARDWARE;
    Settings.Timeout_Sec=0;
    Settings.Timeout_Millisec=500;

#ifdef QT_THREAD_SUPPORT
    if (!mutex) {
        mutex=new QMutex(TRUE);
    }
    refCount++;
#endif

    portOpen=false;
}

/*!
\fn bool QextSerialBase::isOpen(void)
Returns true if the port is currently open, false otherwise
*/
bool QextSerialBase::isOpen(void) {
    return portOpen;
}
