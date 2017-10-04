#ifndef DATABASE_H
#define DATABASE_H
#include<bits/stdc++.h>
#include<datatable.h>
#include<datatypes.h>
#include<additional_functions.h>
#include "messagestack.h"

using namespace std;

class DataBase : MessageStack
{
private:
    QString name;
    QString login;
    QString pass;

    bool is_auth = false;
    set<QString> used_names;
    vector<DataTable> tables;
    int active_table = -1;

public:
    DataBase(QString , QString , QString);
    DataBase(QJsonObject);
    DataBase();
    bool createTable(QString , vector<DataTypes> , vector<QString>);
    bool addRow(vector<pair<int , QString>>);
    QJsonDocument toJson();
    bool authentication(QString , QString);
    void addMessage(MessageType , QString);
    set<QString> getTableNamesSet()const;
    QString getName()const;
    QString getLogin()const;
    QString getPassword()const;
    bool setName(QString);
    bool setLogin(QString);
    bool setPassword(QString);
    DataTable getDataTable(QString)const;
    void saveTable(QString tableName , map<pair<int , int> , QString> values);
    bool eraseTable(QString tableName);
    bool addRow(vector<pair<int , QString>> data , QString _name);
    DataTable Intersect(vector<QString>);
};

#endif // DATABASE_H
