#include "datarow.h"
#include<bits/stdc++.h>
#include<QJsonDocument>
#include<QJsonObject>
#include<QDebug>
#include<datatypes.h>
#include<additional_functions.h>

using namespace std;

DataRow::DataRow(vector<DataTypes> & types , vector<pair<int , QString>> & _data)
{
    for(int i = 0 ; i < _data.size() ; ++i){
        data.push_back({_data[i].first , Value(types[i] , _data[i].second)});
    }
    is_sorted = false;
}

DataRow::DataRow(QJsonObject obj){
    for(auto e : obj.keys()){
        int key = e.toInt();
        data.push_back({key , Value(obj.value(e).toObject())});
    }
    is_sorted = false;
}

DataRow::DataRow(DataRow other , set<int> projection){
    for(auto & e : other.data){
        if(projection.find(e.first) != projection.end()){
            this->data.push_back(e);
        }
    }
    is_sorted = false;
}

QJsonValue DataRow::toJson()
{
    QJsonObject obj;
    for(auto e : data){
        obj.insert(QString::number(e.first) , QJsonValue(e.second.toJson()));
    }
    QJsonValue doc(obj);
    return doc;
}

void DataRow::save(QString & filename)
{
 //   saveJson(this->toJson() , filename);
}


vector<pair<int , DataRow::Value>> DataRow::getData()const{
    return this->data;
}

void DataRow::update(int pos , QString val){
    for(int i = 0 ; i < data.size() ; ++i){
        if(data[i].first == pos){
            data[i].second.value = val;
        }
    }
}

void DataRow::sortData(){
    if(is_sorted)return;
    sort(data.begin() , data.end() ,
        [](const pair<int , Value> & a, const pair<int , Value> & b) -> bool
        {
            return a.first < b.first;
        }
    );
    is_sorted = true;
}

bool DataRow::operator==(DataRow & other){
    if(this->data.size() != other.data.size()){
        return false;
    }

    sortData();
    other.sortData();

    bool res = 1;
    for(int i = 0 ; i < data.size() ; ++i){
        res &= (data[i].first == other.data[i].first);
        res &= (data[i].second.type == other.data[i].second.type);
        res &= (data[i].second.value == other.data[i].second.value);
    }
    return res;
}

DataRow DataRow::concat(DataRow row1 , DataRow row2 , map<int , int> code){
    for(auto &e : row2.data){
        row1.data.push_back({code[e.first] , e.second});
    }
    return row1;
}
