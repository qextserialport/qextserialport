/**
 * @file main.cpp
 * @brief Main file.
 * @author Micha? Policht
 */

#include <QApplication>
#include "MainWindow.h"
#include "MessageWindow.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
    //! [0]
    //redirect debug messages to the MessageWindow dialog
	qInstallMsgHandler(MessageWindow::AppendMsgWrapper);
    //! [0]

    MainWindow mainWindow;
	mainWindow.show();

    return app.exec();
}


