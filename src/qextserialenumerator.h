/*!
 * \file qextserialenumerator.h
 * \author Michal Policht
 * \see QextSerialEnumerator
 */

#ifndef _QEXTSERIALENUMERATOR_H_
#define _QEXTSERIALENUMERATOR_H_

#include <QtCore/QList>
#include <QtCore/QObject>
#include "qextserialport_global.h"

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

class QextSerialEnumeratorPrivate;
class QEXTSERIALPORT_EXPORT QextSerialEnumerator : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QextSerialEnumerator)
public:
    QextSerialEnumerator(QObject * parent=0);
    ~QextSerialEnumerator();

    static QList<QextPortInfo> getPorts();
    void setUpNotifications();

signals:
    void deviceDiscovered(const QextPortInfo & info);
    void deviceRemoved(const QextPortInfo & info);

private:
    Q_DISABLE_COPY(QextSerialEnumerator)
    QextSerialEnumeratorPrivate *d_ptr;
};

#endif /*_QEXTSERIALENUMERATOR_H_*/
