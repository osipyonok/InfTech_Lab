#include "mainwindow.h"
#include <QApplication>
#include <databasesocket.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DataBaseSocket *socket = new DataBaseSocket();


    MainWindow w(socket);
    w.show();

    return a.exec();
}
