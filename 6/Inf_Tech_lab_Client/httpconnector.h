#ifndef HTTPCONNECTOR_H
#define HTTPCONNECTOR_H
#include <bits/stdc++.h>
#include <QDebug>
#include <QString>
#include <QThread>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QObject>
#include <QByteArray>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>

using namespace std;

class HttpConnector : public QThread
{
    Q_OBJECT
public:
    explicit HttpConnector();
    void run();

signals:
    void gotData(QByteArray arr);

public slots:
    void replyFinish(QNetworkReply* reply);

public:
    QString descriptor;
    QNetworkAccessManager * pManager;
};

#endif // HTTPCONNECTOR_H
