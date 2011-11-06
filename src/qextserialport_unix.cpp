#include "qextserialport.h"
#include "qextserialport_p.h"
#include <fcntl.h>
#include <stdio.h>

#include <QMutexLocker>
#include <QDebug>

void QextSerialPortPrivate::platformSpecificInit()
{
    fd = 0;
    readNotifier = 0;
}

/*!
    Standard destructor.
*/
void QextSerialPortPrivate::platformSpecificDestruct()
{
}


void QextSerialPort::setTimeout(long millisec)
{
    QMutexLocker lock(mutex);

}

bool QextSerialPortPrivate::open_sys(QIODevice::OpenMode mode)
{
    QMutexLocker lock(mutex);
    qDebug() << "trying to open file" << port.toAscii();
    //note: linux 2.6.21 seems to ignore O_NDELAY flag
    if ((fd = ::open(port.toAscii() ,O_RDWR | O_NOCTTY | O_NDELAY)) != -1) {
        qDebug("file opened succesfully");

        QIODevice::open(mode);          // Flag the port as opened
        tcgetattr(fd, &old_termios);    // Save the old termios
        Posix_CommConfig = old_termios; // Make a working copy
        cfmakeraw(&Posix_CommConfig);   // Enable raw access

        /*set up other port settings*/
        Posix_CommConfig.c_cflag|=CREAD|CLOCAL;
        Posix_CommConfig.c_lflag&=(~(ICANON|ECHO|ECHOE|ECHOK|ECHONL|ISIG));
        Posix_CommConfig.c_iflag&=(~(INPCK|IGNPAR|PARMRK|ISTRIP|ICRNL|IXANY));
        Posix_CommConfig.c_oflag&=(~OPOST);
        Posix_CommConfig.c_cc[VMIN]= 0;
#ifdef _POSIX_VDISABLE  // Is a disable character available on this system?
        // Some systems allow for per-device disable-characters, so get the
        //  proper value for the configured device
        const long vdisable = fpathconf(fd, _PC_VDISABLE);
        Posix_CommConfig.c_cc[VINTR] = vdisable;
        Posix_CommConfig.c_cc[VQUIT] = vdisable;
        Posix_CommConfig.c_cc[VSTART] = vdisable;
        Posix_CommConfig.c_cc[VSTOP] = vdisable;
        Posix_CommConfig.c_cc[VSUSP] = vdisable;
#endif //_POSIX_VDISABLE
        setBaudRate(Settings.BaudRate);
        setDataBits(Settings.DataBits);
        setParity(Settings.Parity);
        setStopBits(Settings.StopBits);
        setFlowControl(Settings.FlowControl);
        setTimeout(Settings.Timeout_Millisec);
        tcsetattr(fd, TCSAFLUSH, &Posix_CommConfig);

        if (queryMode() == QextSerialPort::EventDriven) {
            readNotifier = new QSocketNotifier(fd, QSocketNotifier::Read, this);
            connect(readNotifier, SIGNAL(activated(int)), this, SIGNAL(readyRead()));
        }
        return true;
    } else {
        qDebug() << "could not open file:" << QString::fromLocal8Bit(strerror(errno));
        lastErr = E_FILE_NOT_FOUND;
        return false;
    }
}

void QextSerialPortPrivate::close_sys()
{
    QMutexLocker lock(mutex);
    // Force a flush and then restore the original termios
    flush_sys();
    // Using both TCSAFLUSH and TCSANOW here discards any pending input
    tcsetattr(fd, TCSAFLUSH | TCSANOW, &old_termios);   // Restore termios
    ::close(fd);
    if(readNotifier) {
        delete readNotifier;
        readNotifier = 0;
    }
}

void QextSerialPortPrivate::flush_sys()
{
    QMutexLocker lock(mutex);
    tcflush(fd, TCIOFLUSH);
}

qint64 QextSerialPortPrivate::bytesAvailable_sys() const
{
    QMutexLocker lock(mutex);
    int bytesQueued;
    if (ioctl(fd, FIONREAD, &bytesQueued) == -1) {
        return (qint64)-1;
    }
    return bytesQueued + QIODevice::bytesAvailable();
}

/*!
    Translates a system-specific error code to a QextSerialPort error code.  Used internally.
*/
void QextSerialPortPrivate::translateError(ulong error)
{
    switch (error) {
        case EBADF:
        case ENOTTY:
            lastErr=E_INVALID_FD;
            break;

        case EINTR:
            lastErr=E_CAUGHT_NON_BLOCKED_SIGNAL;
            break;

        case ENOMEM:
            lastErr=E_NO_MEMORY;
            break;
    }
}

void QextSerialPortPrivate::setDtr_sys(bool set)
{
    QMutexLocker lock(mutex);
    int status;
    ioctl(fd, TIOCMGET, &status);
    if (set) {
        status|=TIOCM_DTR;
    }
    else {
        status&=~TIOCM_DTR;
    }
    ioctl(fd, TIOCMSET, &status);
}

void QextSerialPortPrivate::setRts_sys(bool set)
{
    QMutexLocker lock(mutex);
    int status;
    ioctl(fd, TIOCMGET, &status);
    if (set) {
        status|=TIOCM_RTS;
    }
    else {
        status&=~TIOCM_RTS;
    }
    ioctl(fd, TIOCMSET, &status);
}

unsigned long QextSerialPortPrivate::lineStatus_sys()
{
    unsigned long Status=0, Temp=0;
    QMutexLocker lock(mutex);
    ioctl(fd, TIOCMGET, &Temp);
    if (Temp&TIOCM_CTS) Status|=LS_CTS;
    if (Temp&TIOCM_DSR) Status|=LS_DSR;
    if (Temp&TIOCM_RI)  Status|=LS_RI;
    if (Temp&TIOCM_CD)  Status|=LS_DCD;
    if (Temp&TIOCM_DTR) Status|=LS_DTR;
    if (Temp&TIOCM_RTS) Status|=LS_RTS;
    if (Temp&TIOCM_ST)  Status|=LS_ST;
    if (Temp&TIOCM_SR)  Status|=LS_SR;
    return Status;
}

/*!
    Reads a block of data from the serial port.  This function will read at most maxSize bytes from
    the serial port and place them in the buffer pointed to by data.  Return value is the number of
    bytes actually read, or -1 on error.
    
    \warning before calling this function ensure that serial port associated with this class
    is currently open (use isOpen() function to check if port is open).
*/
qint64 QextSerialPortPrivate::readData_sys(char * data, qint64 maxSize)
{
    QMutexLocker lock(mutex);
    int retVal = ::read(fd, data, maxSize);
    if (retVal == -1)
        lastErr = E_READ_FAILED;

    return retVal;
}

/*!
    Writes a block of data to the serial port.  This function will write maxSize bytes
    from the buffer pointed to by data to the serial port.  Return value is the number
    of bytes actually written, or -1 on error.
    
    \warning before calling this function ensure that serial port associated with this class
    is currently open (use isOpen() function to check if port is open).
*/
qint64 QextSerialPortPrivate::writeData_sys(const char * data, qint64 maxSize)
{
    QMutexLocker lock(mutex);
    int retVal = ::write(fd, data, maxSize);
    if (retVal == -1)
       lastErr = E_WRITE_FAILED;

    return (qint64)retVal;
}

static void setBaudRate(termios *config, int baudRate)
{
#ifdef CBAUD
                config->c_cflag&=(~CBAUD);
                config->c_cflag|=baudRate;
#else
                cfsetispeed(config, baudRate);
                cfsetospeed(config, baudRate);
#endif
}

void QextSerialPortPrivate::updatePortSettings()
{
    if (!q_ptr->isOpen() || !settingsDirtyFlags)
        return;

    if (settingsDirtyFlags & DFE_BaudRate) {
        switch (Settings.BaudRate) {
        case BAUD50:
            setBaudRate(&Posix_CommConfig, B50);
            break;
        case BAUD75:
            setBaudRate(&Posix_CommConfig, B75);
            break;
        case BAUD110:
            setBaudRate(&Posix_CommConfig, B110);
            break;
        case BAUD134:
            setBaudRate(&Posix_CommConfig, B134);
            break;
        case BAUD150:
            setBaudRate(&Posix_CommConfig, B150);
            break;
        case BAUD200:
            setBaudRate(&Posix_CommConfig, B200);
            break;
        case BAUD300:
            setBaudRate(&Posix_CommConfig, B300);
            break;
        case BAUD600:
            setBaudRate(&Posix_CommConfig, B600);
            break;
        case BAUD1200:
            setBaudRate(&Posix_CommConfig, B1200);
            break;
        case BAUD1800:
            setBaudRate(&Posix_CommConfig, B1800);
            break;
        case BAUD2400:
            setBaudRate(&Posix_CommConfig, B2400);
            break;
        case BAUD4800:
            setBaudRate(&Posix_CommConfig, B4800);
            break;
        case BAUD9600:
            setBaudRate(&Posix_CommConfig, B9600);
            break;
        case BAUD19200:
            setBaudRate(&Posix_CommConfig, B19200);
            break;
        case BAUD38400:
            setBaudRate(&Posix_CommConfig, B38400);
            break;
        case BAUD57600:
            setBaudRate(&Posix_CommConfig, B57600);
            break;
#ifdef B76800
        case BAUD76800:
            setBaudRate(&Posix_CommConfig, B76800);
            break;
#endif
        case BAUD115200:
            setBaudRate(&Posix_CommConfig, B115200);
            break;
        }
    }
    if (settingsDirtyFlags & DFE_Parity) {
        switch (Settings.Parity) {
        /*space parity*/
        case PAR_SPACE:
            /*space parity not directly supported - add an extra data bit to simulate it*/
            Posix_CommConfig.c_cflag&=~(PARENB|CSIZE);
            switch(Settings.DataBits) {
            case DATA_5:
                Posix_CommConfig.c_cflag|=CS6;
                break;
            case DATA_6:
                Posix_CommConfig.c_cflag|=CS7;
                break;
            case DATA_7:
                Posix_CommConfig.c_cflag|=CS8;
                break;
            }
            break;
        /*no parity*/
        case PAR_NONE:
            Posix_CommConfig.c_cflag&=(~PARENB);
            break;
        /*even parity*/
        case PAR_EVEN:
            Posix_CommConfig.c_cflag&=(~PARODD);
            Posix_CommConfig.c_cflag|=PARENB;
            break;
        /*odd parity*/
        case PAR_ODD:
            Posix_CommConfig.c_cflag|=(PARENB|PARODD);
            break;
        }
    }
    if (settingsDirtyFlags & DFE_DataBits) {
        Posix_CommConfig.c_cflag&=(~CSIZE);
        switch(Settings.DataBits) {
        case DATA_5:
            Posix_CommConfig.c_cflag|=CS5;
            break;
        case DATA_6:
            Posix_CommConfig.c_cflag|=CS6;
            break;
        case DATA_7:
            Posix_CommConfig.c_cflag|=CS7;
            break;
        case DATA_8:
            Posix_CommConfig.c_cflag|=CS8;
            break;
        }
    }
    if (settingsDirtyFlags & DFE_StopBits) {
        switch (Settings.StopBits) {
        /*one stop bit*/
        case STOP_1:
            Settings.StopBits=stopBits;
            Posix_CommConfig.c_cflag&=(~CSTOPB);
            break;
        /*two stop bits*/
        case STOP_2:
            Settings.StopBits=stopBits;
            Posix_CommConfig.c_cflag|=CSTOPB;
            break;
        }
    }
    if (settingsDirtyFlags & DFE_Flow) {
        switch(flow) {
        /*no flow control*/
        case FLOW_OFF:
            Posix_CommConfig.c_cflag&=(~CRTSCTS);
            Posix_CommConfig.c_iflag&=(~(IXON|IXOFF|IXANY));
            break;
        /*software (XON/XOFF) flow control*/
        case FLOW_XONXOFF:
            Posix_CommConfig.c_cflag&=(~CRTSCTS);
            Posix_CommConfig.c_iflag|=(IXON|IXOFF|IXANY);
            break;
        case FLOW_HARDWARE:
            Posix_CommConfig.c_cflag|=CRTSCTS;
            Posix_CommConfig.c_iflag&=(~(IXON|IXOFF|IXANY));
            break;
        }
    }

    if (settingsDirtyFlags & DFE_Settings_Mask)
        tcsetattr(fd, TCSAFLUSH, &Posix_CommConfig);

    if (settingsDirtyFlags & DFE_TimeOut) {
        int millisec = Settings.Timeout_Millisec;
        Posix_Copy_Timeout.tv_sec = millisec / 1000;
        Posix_Copy_Timeout.tv_usec = millisec % 1000;
        if (millisec == -1) {
            fcntl(fd, F_SETFL, O_NDELAY);
        }
        else {
            //O_SYNC should enable blocking ::write()
            //however this seems not working on Linux 2.6.21 (works on OpenBSD 4.2)
            fcntl(fd, F_SETFL, O_SYNC);
        }
        tcgetattr(fd, & Posix_CommConfig);
        Posix_CommConfig.c_cc[VTIME] = millisec/100;
        tcsetattr(fd, TCSAFLUSH, & Posix_CommConfig);
    }

    settingsDirtyFlags = 0;
}
