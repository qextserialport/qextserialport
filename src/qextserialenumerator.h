/*!
 * \file qextserialenumerator.h
 * \author Michal Policht
 * \see QextSerialEnumerator
 */

#ifndef _QEXTSERIALENUMERATOR_H_
#define _QEXTSERIALENUMERATOR_H_


#include <QString>
#include <QList>
#include <QObject>
#include "qextserialport_global.h"

#ifdef Q_OS_WIN
    //needed for mingw to pull in appropriate dbt business...
    //probably a better way to do this
    //http://mingw-users.1079350.n2.nabble.com/DEV-BROADCAST-DEVICEINTERFACE-was-not-declared-in-this-scope-td3552762.html
    #ifdef  __MINGW32__
        #define _WIN32_WINNT 0x0500
        #define _WIN32_WINDOWS 0x0500
        #define WINVER 0x0500
    #endif
    #include <windows.h>
    #include <setupapi.h>
    #include <dbt.h>
#endif /*Q_OS_WIN*/

#ifdef Q_OS_MAC
    #include <IOKit/usb/IOUSBLib.h>
#endif /*Q_OS_MAC*/

/*!
 * Structure containing port information.
 */
struct QextPortInfo {
    QString portName;   ///< Port name.
    QString physName;   ///< Physical name.
    QString friendName; ///< Friendly name.
    QString enumName;   ///< Enumerator name.
    int vendorID;       ///< Vendor ID.
    int productID;      ///< Product ID
};

#ifdef Q_OS_WIN
#ifdef QT_GUI_LIB
#include <QWidget>
class QextSerialEnumerator;

class QextSerialRegistrationWidget : public QWidget
{
    Q_OBJECT
public:
    QextSerialRegistrationWidget( QextSerialEnumerator* qese ) {
        this->qese = qese;
    }
    ~QextSerialRegistrationWidget( ) { }

protected:
    QextSerialEnumerator* qese;
    bool winEvent( MSG* message, long* result );
};
#endif // QT_GUI_LIB
#endif // Q_OS_WIN

class QEXTSERIALPORT_EXPORT QextSerialEnumerator : public QObject
{
    Q_OBJECT
public:
    QextSerialEnumerator( );
    ~QextSerialEnumerator( );

#ifdef Q_OS_WIN
    LRESULT onDeviceChangeWin( WPARAM wParam, LPARAM lParam );
private:
    static QString getRegKeyValue(HKEY key, LPCTSTR property);
    static QString getDeviceProperty(HDEVINFO devInfo, PSP_DEVINFO_DATA devData, DWORD property);

    /*!
    * Search for serial ports using setupapi.
    *  \param infoList list with result.
    */
    static void setupAPIScan(QList<QextPortInfo> & infoList);
    void setUpNotificationWin( );
    static bool getDeviceDetailsWin( QextPortInfo* portInfo, HDEVINFO devInfo,
                                    PSP_DEVINFO_DATA devData, WPARAM wParam = DBT_DEVICEARRIVAL );
    static void enumerateDevicesWin( const GUID & guidDev, QList<QextPortInfo>* infoList );
    bool matchAndDispatchChangedDevice(const QString & deviceID, const GUID & guid, WPARAM wParam);
#ifdef QT_GUI_LIB
    QextSerialRegistrationWidget* notificationWidget;
#endif
#endif /*Q_OS_WIN*/

#ifdef Q_OS_UNIX
#ifdef Q_OS_MAC
private:
    /*!
     * Search for serial ports using IOKit.
     *    \param infoList list with result.
     */
    static void scanPortsOSX(QList<QextPortInfo> & infoList);
    static void iterateServicesOSX(io_object_t service, QList<QextPortInfo> & infoList);
    static bool getServiceDetailsOSX( io_object_t service, QextPortInfo* portInfo );

    void setUpNotificationOSX( );
    void onDeviceDiscoveredOSX( io_object_t service );
    void onDeviceTerminatedOSX( io_object_t service );
    friend void deviceDiscoveredCallbackOSX( void *ctxt, io_iterator_t serialPortIterator );
    friend void deviceTerminatedCallbackOSX( void *ctxt, io_iterator_t serialPortIterator );

    IONotificationPortRef notificationPortRef;

#else // Q_OS_MAC
private:
    /*!
     * Search for serial ports on unix.
     *    \param infoList list with result.
     */
    static void scanPortsNix(QList<QextPortInfo> & infoList);
#endif // Q_OS_MAC
#endif /* Q_OS_UNIX */

public:
    static QList<QextPortInfo> getPorts();
    void setUpNotifications( );

signals:
    void deviceDiscovered( const QextPortInfo & info );
    void deviceRemoved( const QextPortInfo & info );
};

#endif /*_QEXTSERIALENUMERATOR_H_*/
