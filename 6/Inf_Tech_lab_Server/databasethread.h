#ifndef DATABASETHREAD_H
#define DATABASETHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QSet>
#include "databaseconnection.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

class DataBaseThread : public QThread
{
    Q_OBJECT
public:
    explicit DataBaseThread(qintptr id , QSet<QString> *usedDBs , QObject *parent = 0);
    void run();
    ~DataBaseThread(){
    //    this->usedDBs->remove(this->connection->getDataBaseName());
    }

signals:
    void error(QTcpSocket::SocketError socketError);

public slots:
    void readyRead();
    void disconnect();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
    QSet<QString> *usedDBs;
    DataBaseConnection *connection;
};

#endif // DATABASETHREAD_H
