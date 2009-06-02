/**
 * @file qextserialenumerator.cpp
 * @author Michał Policht
 * @see QextSerialEnumerator
 */

#include "qextserialenumerator.h"
#include <QDebug>
#include <QMetaType>

QextSerialEnumerator::QextSerialEnumerator( )
{
    if( !QMetaType::isRegistered( QMetaType::type("QextPortInfo") ) )
        qRegisterMetaType<QextPortInfo>("QextPortInfo");
}

QextSerialEnumerator::~QextSerialEnumerator( )
{
#ifdef Q_OS_MAC
    IONotificationPortDestroy( notificationPortRef );
#endif
}

#ifdef _TTY_WIN_

    #include <objbase.h>
    #include <initguid.h>


    //this is serial port GUID
    #ifndef GUID_CLASS_COMPORT
        DEFINE_GUID(GUID_CLASS_COMPORT, 0x86e0d1e0L, 0x8089, 0x11d0, 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73);
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
        #define TCHARToQString(x)     QString::fromLocal8Bit((x))
        #define TCHARToQStringN(x,y)  QString::fromLocal8Bit((x),(y))
    #endif /*UNICODE*/


    //static
    QString QextSerialEnumerator::getRegKeyValue(HKEY key, LPCTSTR property)
    {
        DWORD size = 0;
        RegQueryValueEx(key, property, NULL, NULL, NULL, & size);
        BYTE * buff = new BYTE[size];
        if (RegQueryValueEx(key, property, NULL, NULL, buff, & size) == ERROR_SUCCESS) {
            return TCHARToQStringN(buff, size);
            delete [] buff;
        } else {
            qWarning("QextSerialEnumerator::getRegKeyValue: can not obtain value from registry");
            delete [] buff;
            return QString();
        }
    }

    //static
    QString QextSerialEnumerator::getDeviceProperty(HDEVINFO devInfo, PSP_DEVINFO_DATA devData, DWORD property)
    {
        DWORD buffSize = 0;
        SetupDiGetDeviceRegistryProperty(devInfo, devData, property, NULL, NULL, 0, & buffSize);
        BYTE * buff = new BYTE[buffSize];
        if (!SetupDiGetDeviceRegistryProperty(devInfo, devData, property, NULL, buff, buffSize, NULL))
            qCritical("Can not obtain property: %ld from registry", property);
        QString result = TCHARToQString(buff);
        delete [] buff;
        return result;
    }

    //static
    void QextSerialEnumerator::setupAPIScan(QList<QextPortInfo> & infoList)
    {
        HDEVINFO devInfo = INVALID_HANDLE_VALUE;
        GUID * guidDev = (GUID *) & GUID_CLASS_COMPORT;

        devInfo = SetupDiGetClassDevs(guidDev, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
        if(devInfo == INVALID_HANDLE_VALUE) {
            qCritical("SetupDiGetClassDevs failed. Error code: %ld", GetLastError());
            return;
        }

        //enumerate the devices
        bool ok = true;
        SP_DEVICE_INTERFACE_DATA ifcData;
        ifcData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
        SP_DEVICE_INTERFACE_DETAIL_DATA * detData = NULL;
        DWORD detDataSize = 0;
        DWORD oldDetDataSize = 0;

        for (DWORD i = 0; ok; i++) {
            ok = SetupDiEnumDeviceInterfaces(devInfo, NULL, guidDev, i, &ifcData);
            if (ok) {
                SP_DEVINFO_DATA devData = {sizeof(SP_DEVINFO_DATA)};
                //check for required detData size
                SetupDiGetDeviceInterfaceDetail(devInfo, & ifcData, NULL, 0, & detDataSize, & devData);
                //if larger than old detData size then reallocate the buffer
                if (detDataSize > oldDetDataSize) {
                    delete [] detData;
                    detData = (SP_DEVICE_INTERFACE_DETAIL_DATA *) new char[detDataSize];
                    detData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
                    oldDetDataSize = detDataSize;
                }
                //check the details
                if (SetupDiGetDeviceInterfaceDetail(devInfo, & ifcData, detData, detDataSize,
                                                    NULL, & devData)) {
                    // Got a device. Get the details.
                    QextPortInfo info;
                    info.friendName = getDeviceProperty(devInfo, & devData, SPDRP_FRIENDLYNAME);
                    info.physName = getDeviceProperty(devInfo, & devData, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME);
                    info.enumName = getDeviceProperty(devInfo, & devData, SPDRP_ENUMERATOR_NAME);
                    //anyway, to get the port name we must still open registry directly :( ???
                    //Eh...
                    HKEY devKey = SetupDiOpenDevRegKey(devInfo, & devData, DICS_FLAG_GLOBAL, 0,
                                                        DIREG_DEV, KEY_READ);
                    info.portName = getRegKeyValue(devKey, TEXT("PortName"));
                    RegCloseKey(devKey);
                    infoList.append(info);
                } else {
                    qCritical("SetupDiGetDeviceInterfaceDetail failed. Error code: %ld", GetLastError());
                    delete [] detData;
                    return;
                }
            } else {
                if (GetLastError() != ERROR_NO_MORE_ITEMS) {
                    delete [] detData;
                    qCritical("SetupDiEnumDeviceInterfaces failed. Error code: %ld", GetLastError());
                    return;
                }
            }
        }
        delete [] detData;
    }

#endif /*_TTY_WIN_*/

    //#ifdef _TTY_POSIX_

#ifdef Q_OS_MAC
#include <IOKit/serial/IOSerialKeys.h>
#include <CoreFoundation/CFNumber.h>
#include <sys/param.h>

// static
void QextSerialEnumerator::scanPortsOSX(QList<QextPortInfo> & infoList)
{
    io_iterator_t serialPortIterator = 0;
    kern_return_t kernResult = KERN_FAILURE;
    CFMutableDictionaryRef matchingDictionary;

    // first try to get any serialbsd devices, then try any USBCDC devices
    if( !(matchingDictionary = IOServiceMatching(kIOSerialBSDServiceValue) ) ) {
        qWarning("IOServiceMatching returned a NULL dictionary.");
        return;
    }
    CFDictionaryAddValue(matchingDictionary, CFSTR(kIOSerialBSDTypeKey), CFSTR(kIOSerialBSDAllTypes));

    // then create the iterator with all the matching devices
    if( IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDictionary, &serialPortIterator) != KERN_SUCCESS ) {
        qCritical() << "IOServiceGetMatchingServices failed, returned" << kernResult;
        return;
    }
    iterateServicesOSX(serialPortIterator, infoList);
    IOObjectRelease(serialPortIterator);
    serialPortIterator = 0;

    if( !(matchingDictionary = IOServiceNameMatching("AppleUSBCDC")) ) {
        qWarning("IOServiceNameMatching returned a NULL dictionary.");
        return;
    }

    if( IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDictionary, &serialPortIterator) != KERN_SUCCESS ) {
        qCritical() << "IOServiceGetMatchingServices failed, returned" << kernResult;
        return;
    }
    iterateServicesOSX(serialPortIterator, infoList);
    IOObjectRelease(serialPortIterator);
}

void QextSerialEnumerator::iterateServicesOSX(io_object_t service, QList<QextPortInfo> & infoList)
{
    // Iterate through all modems found.
    io_object_t usbService;
    while( ( usbService = IOIteratorNext(service) ) )
    {
        QextPortInfo info;
        info.vendorID = 0;
        info.productID = 0;
        getServiceDetailsOSX( usbService, &info );
        infoList.append(info);
    }
}

bool QextSerialEnumerator::getServiceDetailsOSX( io_object_t service, QextPortInfo* portInfo )
{
    bool retval = true;
    CFTypeRef bsdPathAsCFString = NULL;
    CFTypeRef productNameAsCFString = NULL;
    CFTypeRef vendorIdAsCFNumber = NULL;
    CFTypeRef productIdAsCFNumber = NULL;
    // check the name of the modem's callout device
    bsdPathAsCFString = IORegistryEntryCreateCFProperty(service, CFSTR(kIOCalloutDeviceKey),
                                                        kCFAllocatorDefault, 0);

    // wander up the hierarchy until we find the level that can give us the
    // vendor/product IDs and the product name, if available
    io_registry_entry_t parent;
    kern_return_t kernResult = IORegistryEntryGetParentEntry(service, kIOServicePlane, &parent);
    while( kernResult == KERN_SUCCESS && !vendorIdAsCFNumber && !productIdAsCFNumber )
    {
        if(!productNameAsCFString)
            productNameAsCFString = IORegistryEntrySearchCFProperty(parent,
                                                                    kIOServicePlane,
                                                                    CFSTR("Product Name"),
                                                                    kCFAllocatorDefault, 0);
        vendorIdAsCFNumber = IORegistryEntrySearchCFProperty(parent,
                                                             kIOServicePlane,
                                                             CFSTR(kUSBVendorID),
                                                             kCFAllocatorDefault, 0);
        productIdAsCFNumber = IORegistryEntrySearchCFProperty(parent,
                                                              kIOServicePlane,
                                                              CFSTR(kUSBProductID),
                                                              kCFAllocatorDefault, 0);
        io_registry_entry_t oldparent = parent;
        kernResult = IORegistryEntryGetParentEntry(parent, kIOServicePlane, &parent);
        IOObjectRelease(oldparent);
    }

    io_string_t ioPathName;
    IORegistryEntryGetPath( service, kIOServicePlane, ioPathName );
    portInfo->physName = ioPathName;

    if( bsdPathAsCFString )
    {
        char path[MAXPATHLEN];
        if( CFStringGetCString((CFStringRef)bsdPathAsCFString, path,
                               PATH_MAX, kCFStringEncodingUTF8) )
            portInfo->portName = path;
        CFRelease(bsdPathAsCFString);
    }

    if(productNameAsCFString)
    {
        char productName[MAXPATHLEN];
        if( CFStringGetCString((CFStringRef)productNameAsCFString, productName,
                               PATH_MAX, kCFStringEncodingUTF8) )
            portInfo->friendName = productName;
        CFRelease(productNameAsCFString);
    }

    if(vendorIdAsCFNumber)
    {
        SInt32 vID;
        if(CFNumberGetValue((CFNumberRef)vendorIdAsCFNumber, kCFNumberSInt32Type, &vID))
            portInfo->vendorID = vID;
        CFRelease(vendorIdAsCFNumber);
    }

    if(productIdAsCFNumber)
    {
        SInt32 pID;
        if(CFNumberGetValue((CFNumberRef)productIdAsCFNumber, kCFNumberSInt32Type, &pID))
            portInfo->productID = pID;
        CFRelease(productIdAsCFNumber);
    }
    IOObjectRelease(service);
    return retval;
}

// IOKit callbacks registered via setupNotifications()
void deviceDiscoveredCallbackOSX( void *ctxt, io_iterator_t serialPortIterator );
void deviceTerminatedCallbackOSX( void *ctxt, io_iterator_t serialPortIterator );

void deviceDiscoveredCallbackOSX( void *ctxt, io_iterator_t serialPortIterator )
{
    QextSerialEnumerator* qese = (QextSerialEnumerator*)ctxt;
    io_object_t serialService;
    while ((serialService = IOIteratorNext(serialPortIterator)))
        qese->onDeviceDiscoveredOSX(serialService);
}

void deviceTerminatedCallbackOSX( void *ctxt, io_iterator_t serialPortIterator )
{
    QextSerialEnumerator* qese = (QextSerialEnumerator*)ctxt;
    io_object_t serialService;
    while ((serialService = IOIteratorNext(serialPortIterator)))
        qese->onDeviceTerminatedOSX(serialService);
}

/*
  A device has been discovered via IOKit.
  Create a QextPortInfo if possible, and emit the signal indicating that we've found it.
*/
void QextSerialEnumerator::onDeviceDiscoveredOSX( io_object_t service )
{
    QextPortInfo info;
    info.vendorID = 0;
    info.productID = 0;
    if( getServiceDetailsOSX( service, &info ) )
        emit deviceDiscovered( info );
}

/*
  Notification via IOKit that a device has been removed.
  Create a QextPortInfo if possible, and emit the signal indicating that it's gone.
*/
void QextSerialEnumerator::onDeviceTerminatedOSX( io_object_t service )
{
    QextPortInfo info;
    info.vendorID = 0;
    info.productID = 0;
    if( getServiceDetailsOSX( service, &info ) )
        emit deviceRemoved( info );
}

/*
  Create matching dictionaries for the devices we want to get notifications for,
  and add them to the current run loop.  Invoke the callbacks that will be responding
  to these notifications once to arm them, and discover any devices that
  are currently connected at the time notifications are setup.
*/
void QextSerialEnumerator::setUpNotificationOSX( )
{
    kern_return_t kernResult;
    mach_port_t masterPort;
    CFRunLoopSourceRef notificationRunLoopSource;
    CFMutableDictionaryRef classesToMatch;
    CFMutableDictionaryRef cdcClassesToMatch;
    io_iterator_t portIterator;

    kernResult = IOMasterPort(MACH_PORT_NULL, &masterPort);
    if (KERN_SUCCESS != kernResult) {
        qDebug() << "IOMasterPort returned:" << kernResult;
        return;
    }

    classesToMatch = IOServiceMatching(kIOSerialBSDServiceValue);
    if (classesToMatch == NULL)
        qDebug("IOServiceMatching returned a NULL dictionary.");
    else
        CFDictionarySetValue(classesToMatch, CFSTR(kIOSerialBSDTypeKey), CFSTR(kIOSerialBSDAllTypes));

    if( !(cdcClassesToMatch = IOServiceNameMatching("AppleUSBCDC") ) ) {
        qWarning("couldn't create cdc matching dict");
        return;
    }

    // Retain an additional reference since each call to IOServiceAddMatchingNotification consumes one.
    classesToMatch = (CFMutableDictionaryRef) CFRetain(classesToMatch);
    cdcClassesToMatch = (CFMutableDictionaryRef) CFRetain(cdcClassesToMatch);

    notificationPortRef = IONotificationPortCreate(masterPort);
    if(notificationPortRef == NULL) {
        qDebug("IONotificationPortCreate return a NULL IONotificationPortRef.");
        return;
    }

    notificationRunLoopSource = IONotificationPortGetRunLoopSource(notificationPortRef);
    if (notificationRunLoopSource == NULL) {
        qDebug("IONotificationPortGetRunLoopSource returned NULL CFRunLoopSourceRef.");
        return;
    }

    CFRunLoopAddSource(CFRunLoopGetCurrent(), notificationRunLoopSource, kCFRunLoopDefaultMode);

    kernResult = IOServiceAddMatchingNotification(notificationPortRef, kIOMatchedNotification, classesToMatch,
                                                  deviceDiscoveredCallbackOSX, this, &portIterator);
    if (kernResult != KERN_SUCCESS) {
        qDebug() << "IOServiceAddMatchingNotification return:" << kernResult;
        return;
    }

    // arm the callback, and grab any devices that are already connected
    deviceDiscoveredCallbackOSX( this, portIterator );

    kernResult = IOServiceAddMatchingNotification(notificationPortRef, kIOMatchedNotification, cdcClassesToMatch,
                                                  deviceDiscoveredCallbackOSX, this, &portIterator);
    if (kernResult != KERN_SUCCESS) {
        qDebug() << "IOServiceAddMatchingNotification return:" << kernResult;
        return;
    }

    // arm the callback, and grab any devices that are already connected
    deviceDiscoveredCallbackOSX( this, portIterator );

    kernResult = IOServiceAddMatchingNotification(notificationPortRef, kIOTerminatedNotification, classesToMatch,
                                                  deviceTerminatedCallbackOSX, this, &portIterator);
    if (kernResult != KERN_SUCCESS) {
        qDebug() << "IOServiceAddMatchingNotification return:" << kernResult;
        return;
    }

    // arm the callback, and clear any devices that are terminated
    deviceTerminatedCallbackOSX( this, portIterator );

    kernResult = IOServiceAddMatchingNotification(notificationPortRef, kIOTerminatedNotification, cdcClassesToMatch,
                                                  deviceTerminatedCallbackOSX, this, &portIterator);
    if (kernResult != KERN_SUCCESS) {
        qDebug() << "IOServiceAddMatchingNotification return:" << kernResult;
        return;
    }

    // arm the callback, and clear any devices that are terminated
    deviceTerminatedCallbackOSX( this, portIterator );
}
#endif // Q_OS_MAC

//#endif // _TTY_POSIX_

//static
QList<QextPortInfo> QextSerialEnumerator::getPorts()
{
    QList<QextPortInfo> ports;

    #ifdef _TTY_WIN_
        OSVERSIONINFO vi;
        vi.dwOSVersionInfoSize = sizeof(vi);
        if (!::GetVersionEx(&vi)) {
            qCritical("Could not get OS version.");
            return ports;
        }
        // Handle windows 9x and NT4 specially
        if (vi.dwMajorVersion < 5) {
            qCritical("Enumeration for this version of Windows is not implemented yet");
        /*if (vi.dwPlatformId == VER_PLATFORM_WIN32_NT)
                EnumPortsWNt4(ports);
            else
                EnumPortsW9x(ports);*/
        } else  //w2k or later
            setupAPIScan(ports);
    #endif /*_TTY_WIN_*/
    #ifdef _TTY_POSIX_
        #ifdef Q_OS_MAC
            scanPortsOSX(ports);
        #else
            qCritical("Enumeration for POSIX systems is not implemented yet.");
        #endif /* Q_OS_MAC */
    #endif /*_TTY_POSIX_*/

    return ports;
}

void QextSerialEnumerator::setUpNotifications( QMainWindow* win )
{
#ifdef _TTY_WIN_
    setUpNotificationWin( win );
#endif

#ifdef _TTY_POSIX_
#ifdef Q_OS_MAC
    (void)win;
    setUpNotificationOSX( );
#else
    qCritical("Notifications for *Nix/FreeBSD are not implemented yet");
#endif // Q_OS_MAC
#endif // _TTY_POSIX_
}
