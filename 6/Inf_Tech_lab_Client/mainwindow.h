#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <databasesocket.h>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include "createdatabase.h"
#include "opendb.h"
#include <QJsonArray>
#include <dataviewer.h>
#include <QMessageBox>
#include <httpconnector.h>
#include <QNetworkAccessManager>
#include <QObject>
#include <QByteArray>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QQueue>

#define WAIT_FOR_CONNECTION for(volatile int i = 0 ; i < 4e7 ; i += 2)--i;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DataBaseSocket *socket , HttpConnector *conn , QWidget *parent = 0);
    ~MainWindow();
    QQueue<QByteArray> qRequests;

private slots:
    void on_connectButton_clicked();
    void loadDataBases();
    void readyRead(QByteArray arr);
    void replyFinish(QNetworkReply* reply);
    void sendRequests();
    void getResponse();
    void parseResponse(QNetworkReply* reply);

    void on_dataBases_entered(const QModelIndex index);

    void on_createDB_clicked();

    void on_dataBases_clicked(const QModelIndex index);

    void on_dataBases_doubleClicked(const QModelIndex index);

signals:
    void gotData(QByteArray arr);

private:
    Ui::MainWindow *ui;
    DataBaseSocket *socket;
    HttpConnector *connector;
    bool display = false;
    QTimer *getDataBasesTimer;
    QTimer *runCommandTimer;
    QTimer *getResponseTimer;
    QString last_entered;
    QString descriptor;
};

#endif // MAINWINDOW_H
