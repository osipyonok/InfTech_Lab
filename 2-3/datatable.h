#ifndef DATATABLE_H
#define DATATABLE_H
#include<bits/stdc++.h>
#include<datarow.h>
#include<datatypes.h>
#include<additional_functions.h>
#include "messagestack.h"

using namespace std;


class DataTable : MessageStack
{
private:
    QString name;

    bool is_description_sorted = false;
    void sortDescription();
 //   static bool validate(DataTable & _this , vector<pair<int , DataTypes>> &, vector<pair<int , string>> & , vector<DataTypes> &);

public:
    DataTable(){}
    DataTable(QJsonObject obj);
    vector<DataRow> rows;
    vector<pair<int , DataTypes>> description;
    vector<pair<int , QString>> field_names;
    DataTable(QString , vector<DataTypes> , vector<QString>);
    bool operator==(DataTable &);
    bool addRow(vector<pair<int , QString>>);
    pair<bool , DataTypes> getDataType(int);
    QJsonValue toJson();
    QString getName()const;
    vector<pair<int , DataTypes>> getDescription()const;
    vector<pair<int , QString>> getFieldNames()const;
    vector<DataRow> getRows()const;
    void addMessage(MessageType , QString);
    void updateTable(map<pair<int , int> , QString>);
    static DataTable Intersect(vector<DataTable>);

};

bool validate(DataTable * _this , vector<pair<int , DataTypes>> & descr ,
                                vector<pair<int , QString>> & values , vector<DataTypes> & was);

#endif // DATATABLE_H
