#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QTcpServer>
#include <QSet>
#include <QDir>

class TCP_Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit TCP_Server(QObject *parent = 0);
    void startServer();

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QSet<QString> *usedDBs;
};

#endif // TCP_SERVER_H
