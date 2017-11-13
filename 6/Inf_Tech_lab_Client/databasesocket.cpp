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
}

void DataBaseSocket::disconnected(){
    qDebug() << "disconnected\n";
}

void DataBaseSocket::bytesWritten(qint64 bytes){
    qDebug() << "Got " << bytes << " bytes\n";
}

void DataBaseSocket::readyRead(){
    qDebug() << "reading...\n";
    //QByteArray arr = socket->readAll();
    while(socket->canReadLine()){
        qDebug() << "ffdsfds\n";
        QByteArray arr;
        int cnt = 0;
        do{
            QByteArray tmp = socket->readLine();
            for(auto e : tmp){
                if((char)e == '{')++cnt;
                if((char)e == '}')--cnt;
                arr.append(e);
            }
        }while(cnt != 0);
        if(arr.size() > 0)emit gotData(arr);
    }
    //emit gotData(arr);
    //QString str(arr);
    //qDebug() << str << "\n";
}

void DataBaseSocket::write(QString data){
    socket->write(data.toUtf8());
}
