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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DataBaseSocket *socket , QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_connectButton_clicked();
    void loadDataBases();
    void readyRead(QByteArray arr);

    void on_dataBases_entered(const QModelIndex index);

    void on_createDB_clicked();

    void on_dataBases_clicked(const QModelIndex index);

    void on_dataBases_doubleClicked(const QModelIndex index);

private:
    Ui::MainWindow *ui;
    DataBaseSocket *socket;
    bool display = false;
    QTimer *getDataBasesTimer;
    QString last_entered;
};

#endif // MAINWINDOW_H
