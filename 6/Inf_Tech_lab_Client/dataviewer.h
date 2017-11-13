#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QDialog>
#include<bits/stdc++.h>
#include<QJsonDocument>
#include<QJsonObject>
#include<QFile>
#include<QString>
#include<QRegExp>
#include <databasesocket.h>
#include <QJsonArray>
#include <datatable.h>
#include <datavaluewidget.h>
#include <createform.h>
#include <projectiontable.h>

using namespace std;

const QString SHOW_MAIN_PANEL = "bUIgfdjgnfiuYGF73rf_neurgh34YGugfhdsguyt2hg__!fgds";

namespace Ui {
class DataViewer;
}

class DataViewer : public QDialog
{
    Q_OBJECT

public:
    explicit DataViewer(QQueue<QByteArray> *q, DataBaseSocket *socket , QString name , QString login , QString pass ,
                        QJsonArray tables , QWidget *parent = 0);
    explicit DataViewer(QWidget *parent = 0);
    ~DataViewer();

    void reject();

private slots:
    void on_tablesList_doubleClicked(const QModelIndex &index);

    void on_displayPassword_stateChanged(int arg1);

    void on_dbEditReloadButton_clicked();

    void on_dbEditSaveButton_clicked();

    void on_dbSaveButton_clicked();

    void on_dbSaveTable_clicked();

    void on_dbExitButton_clicked();

    void on_deleteTableButton_clicked();

    void on_dbCreateTable_clicked();

    void on_deleteRowButton_clicked();

    void on_addRowButton_clicked();

    void on_makeProjection_clicked();

    void on_crossJoinButton_clicked();


    void on_tablesList_clicked(const QModelIndex &index);

    void on_dbDeleteButton_clicked();

    void on_intersectionButton_clicked();

public slots:
    void readyRead(QByteArray arr);

private:
    Ui::DataViewer *ui;
    void loadDataToEdit();
    QString dbName;
    QString last_table = "";
    QString login , password;
    DataBaseSocket *socket;
    QJsonArray tables;
    DataTable openedTable;
    map<QString , DataTable> cache;
    QQueue<QByteArray> *qRequests;
    void ShowTable(QString);
    void ShowTable(QJsonObject);
    void ShowTable(DataTable);
    map<pair<int , int> , QString> getMapForSave();

};

#endif // DATAVIEWER_H
