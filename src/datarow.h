#ifndef DATAROW_H
#define DATAROW_H
#include<bits/stdc++.h>
#include<datatypes.h>
#include<QJsonDocument>
#include<QJsonObject>
#include<additional_functions.h>
#include "messagestack.h"

using namespace std;


class DataRow : MessageStack
{
private:
    struct Value{
        DataTypes type;
        QString value;

        Value(DataTypes _t , QString _v){
            type = _t;
            value = _v;
        }

        Value(QJsonObject obj){
            type = to_DataTypes(obj.value("type").toString());
            value = obj.value("value").toString();
        }

        QJsonObject toJson(){
            QString tmp = to_string(type);
            QJsonObject json
            {
                {"type" , QJsonValue(tmp)},
                {"value" , QJsonValue(value)}
            };
            return json;
        }
    };
    bool is_sorted;
    vector<pair<int , Value>> data;
    void sortData();
public:
    DataRow(vector<DataTypes> & , vector<pair<int , QString>> &);
    DataRow(DataRow , set<int>);
    DataRow(QJsonObject obj);
    QJsonValue toJson();
    void save(QString &);
    void addMessage(MessageType , QString);
    vector<pair<int , Value>> getData()const;
    void update(int , QString);
    bool operator==(DataRow & other);
};

#endif // DATAROW_H
