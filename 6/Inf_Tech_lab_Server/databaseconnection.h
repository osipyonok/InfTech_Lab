#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QString>
#include "database.h"
#include <QVector>
#include <QPair>
#include <QDirIterator>
#include <QDebug>
#include <QJsonArray>
#include <additional_functions.h>
#include <bits/stdc++.h>

using namespace std;

#define DATABASE_MAIN_DIRECTORY R"(db/)"

class DataBaseConnection
{
public:
    DataBaseConnection(QSet<QString> *usedDBs);
    QVector<QPair<QString , bool>> getDataBases();
    QByteArray createDB(QString name , QString login , QString pass);
    QByteArray openDB(QString name , QString login , QString pass);
    QByteArray setMainData(QString name , QString login , QString pass);
    QByteArray saveDataBase();
    QByteArray showTable(QString name);
    QByteArray deleteTable(QString name);
    QByteArray saveTable(QString name , QJsonArray arr);
    QByteArray closeDataBase();
    QByteArray addRow(QString name);
    QByteArray createTable(QString name , QJsonArray desc);
    QByteArray deleteDataBase();
    QByteArray getPossibleIntersectionTables(QString tbl);
    QByteArray makeIntersection(QJsonArray arr);
    QString getDataBaseName();
    ~DataBaseConnection(){
    //    this->usedDBs->remove(this->db->getName());
    //    this->db = nullptr;
    }

private:
    DataBase *db = nullptr;
    QString path;
    QSet<QString> *usedDBs;

    QString getDataBasePath(QString dbName);
};

#endif // DATABASECONNECTION_H
