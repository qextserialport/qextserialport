


#include "qextserialenumerator.h"
#include <QDebug>
#include <QMetaType>
#include <objbase.h>
#include <initguid.h>
#include "qextserialport.h"
#include <QRegExp>


/*! \class QextSerialEnumerator

    \brief Provides list of ports available in the system.
  
    \section1 Usage
    To poll the system for a list of connected devices, simply use getPorts().  Each
    QextPortInfo structure will populated with information about the corresponding device.
  
    \bold Example
    \code
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    foreach( QextPortInfo port, ports ) {
        // inspect port...
    }
    \endcode
  
    To enable event-driven notification of device connection events, first call
    setUpNotifications() and then connect to the deviceDiscovered() and deviceRemoved()
    signals.  Event-driven behavior is currently available only on Windows and OS X.
  
    \bold Example
    \code
    QextSerialEnumerator* enumerator = new QextSerialEnumerator();
    connect(enumerator, SIGNAL(deviceDiscovered(const QextPortInfo &)),
               myClass, SLOT(onDeviceDiscovered(const QextPortInfo &)));
    connect(enumerator, SIGNAL(deviceRemoved(const QextPortInfo &)),
               myClass, SLOT(onDeviceRemoved(const QextPortInfo &)));
    \endcode
  
    \section1 Credits
    Windows implementation is based on Zach Gorman's work from
    \l {http://www.codeproject.com}{The Code Project} (\l http://www.codeproject.com/system/setupdi.asp).
  
    OS X implementation, see \l http://developer.apple.com/documentation/DeviceDrivers/Conceptual/AccessingHardware/AH_Finding_Devices/chapter_4_section_2.html
  
    \bold author Michal Policht, Liam Staskawicz
*/

/*!
    \fn void QextSerialEnumerator::deviceDiscovered( const QextPortInfo & info )
    A new device has been connected to the system.
  
    setUpNotifications() must be called first to enable event-driven device notifications.
    Currently only implemented on Windows and OS X.
  
    \a info The device that has been discovered.
*/

/*!
   \fn void QextSerialEnumerator::deviceRemoved( const QextPortInfo & info );
    A device has been disconnected from the system.
  
    setUpNotifications() must be called first to enable event-driven device notifications.
    Currently only implemented on Windows and OS X.
  
    \a info The device that was disconnected.
*/

/*!
  default
*/
QextSerialEnumerator::QextSerialEnumerator( )
{
    if( !QMetaType::isRegistered( QMetaType::type("QextPortInfo") ) )
        qRegisterMetaType<QextPortInfo>("QextPortInfo");
#if (defined QT_GUI_LIB)
    notificationWidget = 0;
#endif // Q_OS_WIN
}

/*!
  default
*/
QextSerialEnumerator::~QextSerialEnumerator( )
{
#if (defined QT_GUI_LIB)
    if( notificationWidget )
        delete notificationWidget;
#endif
}



// see http://msdn.microsoft.com/en-us/library/ms791134.aspx for list of GUID classes
#ifndef GUID_DEVCLASS_PORTS
    DEFINE_GUID(GUID_DEVCLASS_PORTS, 0x4D36E978, 0xE325, 0x11CE, 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18 );
#endif

/* Gordon Schumacher's macros for TCHAR -> QString conversions and vice versa */
#ifdef UNICODE
    #define QStringToTCHAR(x)     (wchar_t*) x.utf16()
    #define PQStringToTCHAR(x)    (wchar_t*) x->utf16()
    #define TCHARToQString(x)     QString::fromUtf16((ushort*)(x))
    #define TCHARToQStringN(x,y)  QString::fromUtf16((ushort*)(x),(y))
#else
    #define QStringToTCHAR(x)     x.local8Bit().constData()
    #define PQStringToTCHAR(x)    x->local8Bit().constData()
    #define TCHARToQString(x)     QString::fromLocal8Bit((char*)(x))
    #define TCHARToQStringN(x,y)  QString::fromLocal8Bit((char*)(x),(y))
#endif /*UNICODE*/


/*!
    \internal
    Get value of specified property from the registry.
        \a key handle to an open key.
        \a property property name.

        return property value.
*/
QString QextSerialEnumerator::getRegKeyValue(HKEY key, LPCTSTR property)
{
    DWORD size = 0;
    DWORD type;
    RegQueryValueEx(key, property, NULL, NULL, NULL, & size);
    BYTE* buff = new BYTE[size];
    QString result;
    if( RegQueryValueEx(key, property, NULL, &type, buff, & size) == ERROR_SUCCESS )
        result = TCHARToQString(buff);
    RegCloseKey(key);
    delete [] buff;
    return result;
}

/*!
     \internal
     Get specific property from registry.
     \a devInfo pointer to the device information set that contains the interface
        and its underlying device. Returned by SetupDiGetClassDevs() function.
     \a devData pointer to an SP_DEVINFO_DATA structure that defines the device instance.
        this is returned by SetupDiGetDeviceInterfaceDetail() function.
     \a property registry property. One of defined SPDRP_* constants.

     return property string.
 */
QString QextSerialEnumerator::getDeviceProperty(HDEVINFO devInfo, PSP_DEVINFO_DATA devData, DWORD property)
{
    DWORD buffSize = 0;
    SetupDiGetDeviceRegistryProperty(devInfo, devData, property, NULL, NULL, 0, & buffSize);
    BYTE* buff = new BYTE[buffSize];
    SetupDiGetDeviceRegistryProperty(devInfo, devData, property, NULL, buff, buffSize, NULL);
    QString result = TCHARToQString(buff);
    delete [] buff;
    return result;
}

/*!
    Get list of ports.

    return list of ports currently available in the system.
*/
QList<QextPortInfo> QextSerialEnumerator::getPorts()
{
    QList<QextPortInfo> ports;
    enumerateDevicesWin(GUID_DEVCLASS_PORTS, &ports);
    return ports;
}

void QextSerialEnumerator::enumerateDevicesWin( const GUID & guid, QList<QextPortInfo>* infoList )
{
    HDEVINFO devInfo;
    if( (devInfo = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT)) != INVALID_HANDLE_VALUE)
    {
        SP_DEVINFO_DATA devInfoData;
        devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        for(int i = 0; SetupDiEnumDeviceInfo(devInfo, i, &devInfoData); i++)
        {
            QextPortInfo info;
            info.productID = info.vendorID = 0;
            getDeviceDetailsWin( &info, devInfo, &devInfoData );
            infoList->append(info);
        }
        SetupDiDestroyDeviceInfoList(devInfo);
    }
}

#ifdef QT_GUI_LIB
bool QextSerialRegistrationWidget::winEvent( MSG* message, long* result )
{
    if ( message->message == WM_DEVICECHANGE ) {
        qese->onDeviceChangeWin( message->wParam, message->lParam );
        *result = 1;
        return true;
    }
    return false;
}
#endif

/*!
    Enable event-driven notifications of board discovery/removal.
*/
void QextSerialEnumerator::setUpNotifications( )
{
    #ifdef QT_GUI_LIB
    if(notificationWidget)
        return;
    notificationWidget = new QextSerialRegistrationWidget(this);

    DEV_BROADCAST_DEVICEINTERFACE dbh;
    ZeroMemory(&dbh, sizeof(dbh));
    dbh.dbcc_size = sizeof(dbh);
    dbh.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    CopyMemory(&dbh.dbcc_classguid, &GUID_DEVCLASS_PORTS, sizeof(GUID));
    if( RegisterDeviceNotification( notificationWidget->winId( ), &dbh, DEVICE_NOTIFY_WINDOW_HANDLE ) == NULL)
        qWarning() << "RegisterDeviceNotification failed:" << GetLastError();
    // setting up notifications doesn't tell us about devices already connected
    // so get those manually
    foreach( QextPortInfo port, getPorts() )
      emit deviceDiscovered( port );
    #else
    qWarning("QextSerialEnumerator: GUI not enabled - can't register for device notifications.");
    #endif // QT_GUI_LIB
}

LRESULT QextSerialEnumerator::onDeviceChangeWin( WPARAM wParam, LPARAM lParam )
{
    if ( DBT_DEVICEARRIVAL == wParam || DBT_DEVICEREMOVECOMPLETE == wParam )
    {
        PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
        if( pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE )
        {
            PDEV_BROADCAST_DEVICEINTERFACE pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
             // delimiters are different across APIs...change to backslash.  ugh.
            QString deviceID = TCHARToQString(pDevInf->dbcc_name).toUpper().replace("#", "\\");

            matchAndDispatchChangedDevice(deviceID, GUID_DEVCLASS_PORTS, wParam);
        }
    }
    return 0;
}

bool QextSerialEnumerator::matchAndDispatchChangedDevice(const QString & deviceID, const GUID & guid, WPARAM wParam)
{
    bool rv = false;
    DWORD dwFlag = (DBT_DEVICEARRIVAL == wParam) ? DIGCF_PRESENT : DIGCF_ALLCLASSES;
    HDEVINFO devInfo;
    if( (devInfo = SetupDiGetClassDevs(&guid,NULL,NULL,dwFlag)) != INVALID_HANDLE_VALUE )
    {
        SP_DEVINFO_DATA spDevInfoData;
        spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        for(int i=0; SetupDiEnumDeviceInfo(devInfo, i, &spDevInfoData); i++)
        {
            DWORD nSize=0 ;
            TCHAR buf[MAX_PATH];
            if ( SetupDiGetDeviceInstanceId(devInfo, &spDevInfoData, buf, MAX_PATH, &nSize) &&
                    deviceID.contains(TCHARToQString(buf))) // we found a match
            {
                rv = true;
                QextPortInfo info;
                info.productID = info.vendorID = 0;
                getDeviceDetailsWin( &info, devInfo, &spDevInfoData, wParam );
                if( wParam == DBT_DEVICEARRIVAL )
                    emit deviceDiscovered(info);
                else if( wParam == DBT_DEVICEREMOVECOMPLETE )
                    emit deviceRemoved(info);
                break;
            }
        }
        SetupDiDestroyDeviceInfoList(devInfo);
    }
    return rv;
}

bool QextSerialEnumerator::getDeviceDetailsWin( QextPortInfo* portInfo, HDEVINFO devInfo, PSP_DEVINFO_DATA devData, WPARAM wParam )
{
    portInfo->friendName = getDeviceProperty(devInfo, devData, SPDRP_FRIENDLYNAME);
    if( wParam == DBT_DEVICEARRIVAL)
        portInfo->physName = getDeviceProperty(devInfo, devData, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME);
    portInfo->enumName = getDeviceProperty(devInfo, devData, SPDRP_ENUMERATOR_NAME);
    QString hardwareIDs = getDeviceProperty(devInfo, devData, SPDRP_HARDWAREID);
    HKEY devKey = SetupDiOpenDevRegKey(devInfo, devData, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);
    portInfo->portName = QextSerialPort::fullPortNameWin( getRegKeyValue(devKey, TEXT("PortName")) );
    QRegExp idRx("VID_(\\w+)&PID_(\\w+)");
    if( hardwareIDs.toUpper().contains(idRx) )
    {
        bool dummy;
        portInfo->vendorID = idRx.cap(1).toInt(&dummy, 16);
        portInfo->productID = idRx.cap(2).toInt(&dummy, 16);
        //qDebug() << "got vid:" << vid << "pid:" << pid;
    }
    return true;
}

