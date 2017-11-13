#ifndef DATABASESOCKET_H
#define DATABASESOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QNetworkAccessManager>


class DataBaseSocket : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseSocket(QObject *parent = 0);
    void doConnect(QString server);

signals:
    void gotData(QByteArray arr);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void write(QString data);

private:
    QTcpSocket *socket;
};

#endif // DATABASESOCKET_H
