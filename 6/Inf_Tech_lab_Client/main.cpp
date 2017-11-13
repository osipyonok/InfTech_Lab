#include "mainwindow.h"
#include <QApplication>
#include <databasesocket.h>
#include <httpconnector.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DataBaseSocket *socket = new DataBaseSocket();
    HttpConnector *connector = new HttpConnector();

    MainWindow w(socket , connector);
    w.show();

    return a.exec();
}
