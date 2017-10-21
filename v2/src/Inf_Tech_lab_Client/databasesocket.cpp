#include "databasesocket.h"

DataBaseSocket::DataBaseSocket(QObject *parent) :
    QObject(parent)
{

}

void DataBaseSocket::doConnect(QString server){
    socket = new QTcpSocket(this);

    connect(socket , SIGNAL(connected()) , this , SLOT(connected()));
    connect(socket , SIGNAL(disconnected()) , this , SLOT(disconnected()));
    connect(socket , SIGNAL(bytesWritten(qint64)) , this , SLOT(bytesWritten(qint64)));
    connect(socket , SIGNAL(readyRead()) , this , SLOT(readyRead()));

    qDebug() << "Connecting...\n";

    socket->connectToHost(server , 1234);

    if(!socket->waitForConnected(5000)){
        qDebug() << "Error: " << socket->errorString() << "\n";
    }
}

void DataBaseSocket::connected(){
 //   qDebug() << "connected!\n";
 //   socket->write("Slava huesos");
}

void DataBaseSocket::disconnected(){
    qDebug() << "disconnected\n";
}

void DataBaseSocket::bytesWritten(qint64 bytes){
    qDebug() << "Got " << bytes << " bytes\n";
}

void DataBaseSocket::readyRead(){
    qDebug() << "reading...\n";
    QByteArray arr = socket->readAll();
    emit gotData(arr);
    QString str(arr);
    qDebug() << str << "\n";
}

void DataBaseSocket::write(QString data){
    socket->write(data.toUtf8());
}
