#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QDialog>
#include<bits/stdc++.h>
#include<QJsonDocument>
#include<QJsonObject>
#include<QFile>
#include<QString>
#include<QRegExp>
#include<additional_functions.h>
#include "messagestack.h"
#include <database.h>
#include <datavaluewidget.h>
#include <createform.h>
#include <projectiontable.h>
#include <intersection.h>

const QString SHOW_MAIN_PANEL = "bUIgfdjgnfiuYGF73rf_neurgh34YGugfhdsguyt2hg__!fgds";

namespace Ui {
class DataViewer;
}

class DataViewer : public QDialog
{
    Q_OBJECT

public:
    explicit DataViewer(QJsonObject obj , QString path , QString login , QString pass , QWidget *parent = 0);
    explicit DataViewer(QWidget *parent = 0);
    void ShowTable(QString);
    ~DataViewer();

private slots:
    void on_tablesList_doubleClicked(const QModelIndex &index);

    void on_displayPassword_stateChanged(int arg1);

    void on_dbEditReloadButton_clicked();

    void on_dbEditSaveButton_clicked();

    void on_dbSaveButton_clicked();

    void on_dbSaveAsButton_clicked();

    void on_dbSaveTable_clicked();

    void on_dbExitButton_clicked();

    void on_deleteTableButton_clicked();

    void on_dbCreateTable_clicked();

    void on_deleteRowButton_clicked();

    void on_addRowButton_clicked();

    void on_makeProjection_clicked();

    void on_IntersectionButton_clicked();

private:
    Ui::DataViewer *ui;
    DataBase db;
    void loadDataToEdit();
    QString dbPath;
    QString last_table = "";
    void ShowTable(DataTable);
};

#endif // DATAVIEWER_H
