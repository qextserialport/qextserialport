#ifndef _QEXTSERIALENUMERATOR_P_H_
#define _QEXTSERIALENUMERATOR_P_H_

#include "qextserialenumerator.h"

#ifdef Q_OS_WIN
// needed for mingw to pull in appropriate dbt business...
// probably a better way to do this
// http://mingw-users.1079350.n2.nabble.com/DEV-BROADCAST-DEVICEINTERFACE-was-not-declared-in-this-scope-td3552762.html
#  ifdef  __MINGW32__
#    define _WIN32_WINNT 0x0500
#    define _WIN32_WINDOWS 0x0500
#    define WINVER 0x0500
#  endif
#  include <qt_windows.h>
#endif /*Q_OS_WIN*/

#ifdef Q_OS_MAC
#  include <IOKit/usb/IOUSBLib.h>
#endif /*Q_OS_MAC*/

class QextSerialRegistrationWidget;
class QextSerialEnumeratorPrivate
{
    Q_DECLARE_PUBLIC(QextSerialEnumerator)
public:
    QextSerialEnumeratorPrivate(QextSerialEnumerator * enumrator);
    ~QextSerialEnumeratorPrivate();
    void platformSpecificInit();
    void platformSpecificDestruct();

    static QList<QextPortInfo> getPorts_sys();
    bool setUpNotifications_sys(bool setup);

#ifdef Q_OS_WIN
    LRESULT onDeviceChanged( WPARAM wParam, LPARAM lParam );
    bool matchAndDispatchChangedDevice(const QString & deviceID, const GUID & guid, WPARAM wParam);
#  ifdef QT_GUI_LIB
    QextSerialRegistrationWidget* notificationWidget;
#  endif
#endif /*Q_OS_WIN*/

#ifdef Q_OS_MAC
    /*!
     * Search for serial ports using IOKit.
     *    \param infoList list with result.
     */
    static void iterateServicesOSX(io_object_t service, QList<QextPortInfo> & infoList);
    static bool getServiceDetailsOSX( io_object_t service, QextPortInfo* portInfo );
    void onDeviceDiscoveredOSX( io_object_t service );
    void onDeviceTerminatedOSX( io_object_t service );
    friend void deviceDiscoveredCallbackOSX( void *ctxt, io_iterator_t serialPortIterator );
    friend void deviceTerminatedCallbackOSX( void *ctxt, io_iterator_t serialPortIterator );

    IONotificationPortRef notificationPortRef;
#endif // Q_OS_MAC

private:
    QextSerialEnumerator * q_ptr;
};

#endif //_QEXTSERIALENUMERATOR_P_H_
