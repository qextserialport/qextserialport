/**
 * @file main.cpp
 * @brief Main file.
 * @author Michal Policht
 */

#include <QCoreApplication>
#include "PortListener.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString portName = "COM1";              // update this to use your port of choice
    PortListener *listener = new PortListener(portName);  // signals get hooked up internally

    // start the event loop and wait for signals
    return app.exec();
}
