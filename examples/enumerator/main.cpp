/**
 * @file main.cpp
 * @brief Main file.
 * @author Micha? Policht
 */

#include <QextSerialEnumerator>
#include <QList>
#include <QtDebug>
//! [0]
int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    qDebug() << "List of ports:";
    for (int i = 0; i < ports.size(); i++) {
        qDebug() << "port name:" << ports.at(i).portName;
        qDebug() << "friendly name:" << ports.at(i).friendName;
        qDebug() << "physical name:" << ports.at(i).physName;
        qDebug() << "enumerator name:" << ports.at(i).enumName;
        qDebug() << "vendor ID:" << QString::number(ports.at(i).vendorID, 16);
        qDebug() << "product ID:" << QString::number(ports.at(i).productID, 16);
        qDebug() << "===================================";
    }
    return EXIT_SUCCESS;
}
//! [0]
