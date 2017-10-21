#include "tcp_server.h"
#include "databasethread.h"

TCP_Server::TCP_Server(QObject *parent) :
    QTcpServer(parent)
{
    if(QDir("db\\").exists() == false){
        QDir().mkdir("db\\");
    }
    usedDBs = new QSet<QString>();
}


void TCP_Server::startServer(){
    int port = 1234;

    if(!this->listen(QHostAddress::Any , port)){
        qDebug() << "Could not start server\n";
    }else{
        qDebug() << "Listening to port " << port << "...\n";
    }
}

void TCP_Server::incomingConnection(qintptr socketDescriptor){
    qDebug() << socketDescriptor << " connected\n";
    DataBaseThread *thread = new DataBaseThread(socketDescriptor , usedDBs , this);
    connect(thread , SIGNAL(finished()) , thread , SLOT(deleteLater()));
    thread->start();
}
