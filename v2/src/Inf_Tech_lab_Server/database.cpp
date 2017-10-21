#include "database.h"
#include<bits/stdc++.h>
#include<datatypes.h>
#include<additional_functions.h>
#include<QDebug>

using namespace std;

DataBase::DataBase(){}

DataBase::DataBase(QString _name , QString _login , QString _pass)
{
    name = _name;
    login = _login;
    pass = _pass;
    is_auth = true;
}

DataBase::DataBase(QJsonObject obj)
{
    if((obj.contains("name") && obj.contains("login") && obj.contains("pass") && obj.contains("tables")) == false){
        throw QString("Невiрний формат вхiдного файлу.");
    }
    name = obj.value("name").toString();
    login = obj.value("login").toString();
    pass = obj.value("pass").toString();
    QJsonObject jtables = obj.value("tables").toObject();
    for(auto e : jtables){
        try{
            auto tmp = e.toObject();
            used_names.insert(tmp.value("name").toString());
            tables.push_back(DataTable(tmp));
        }catch(...){}
    }
}

bool DataBase::createTable(QString _name , vector<DataTypes> _description , vector<QString> _field_names)
{
    if(is_auth == false){
    //    addMessage(Error , "Необхiдно авторизуватися.");
        return false;
    }
    if(used_names.find(_name) != used_names.end()){
    //    addMessage(Error , "Таблиця з такою назвою вже icнує.");
        return false;
    }
    if(_description.size() != _field_names.size() or _field_names.size() == 0){
        return false;
    }
    try{
        DataTable table(_name , _description , _field_names);
        tables.push_back(table);
        active_table = tables.size() - 1;
        used_names.insert(_name);
    //    addMessage(Information , "Таблиця створена.");
        return true;
    }catch(...){}
}

bool DataBase::addRow(vector<pair<int , QString>> data)
{
    if(is_auth == false){
        return false;
    }
    if(active_table == -1){
        return false;
    }
    return tables[active_table].addRow(data);
}

bool DataBase::addRow(vector<pair<int , QString>> data , QString _name)
{
    if(is_auth == false){
        return false;
    }
    int pos = -1;
    for(int i = 0 ; i < tables.size() ; ++i){
        if(tables[i].getName() == _name){
            pos = i;
            break;
        }
    }
    if(pos == -1)return false;

    return tables[pos].addRow(data);
}

QJsonDocument DataBase::toJson(){
    if(is_auth == false){
        return QJsonDocument();
    }
    QJsonObject obj{
        {"name" , QJsonValue(name)},
        {"login" , QJsonValue(login)},
        {"pass" , QJsonValue(pass)}
    };

    QJsonObject jtabl;
    for(int i = 0 ; i < tables.size() ; ++i){
        jtabl.insert(QString::number(i) , tables[i].toJson());
    }
    obj.insert("tables" , QJsonValue(jtabl));
    QJsonDocument doc(obj);
    return doc;
}

bool DataBase::authentication(QString _login , QString _pass){
    return is_auth = (is_auth or (login == _login and pass == _pass));
}

set<QString> DataBase::getTableNamesSet()const{
    if(is_auth == false)return {};
    return this->used_names;
}

QString DataBase::getName()const{
    if(is_auth == false)return "";
    return this->name;
}

QString DataBase::getLogin()const{
    if(is_auth == false)return "";
    return this->login;
}

QString DataBase::getPassword()const{
    if(is_auth == false)return "";
    return this->pass;
}

bool DataBase::setName(QString newName){
    if(is_auth == false)return false;
    this->name = newName;
    return true;
}

bool DataBase::setLogin(QString newLogin){
    if(is_auth == false)return false;
    this->login = newLogin;
    return true;
}

bool DataBase::setPassword(QString newPass){
    if(is_auth == false)return false;
    this->pass = newPass;
    return true;
}

DataTable DataBase::getDataTable(QString tableName)const{
    for(auto e : this->tables){
        if(e.getName() == tableName){
            return e;
        }
    }
}

void DataBase::saveTable(QString tableName , map<pair<int , int> , QString> values){
    for(auto &e : tables){
        if(e.getName() == tableName){
            e.updateTable(values);
            break;
        }
    }
}

bool DataBase::eraseTable(QString tableName){
    if(used_names.find(tableName) != used_names.end()){
        auto pos = tables.end();
        for(auto it = tables.begin() ; it != tables.end() ; ++it){
            if((*it).getName() == tableName){
                pos = it;
                break;
            }
        }
        used_names.erase(tableName);
        if(pos != tables.end()){
            tables.erase(pos);
            return true;
        }
    }
    return false;
}

DataTable DataBase::CrossJoin(vector<QString> pt){
    vector<DataTable> v;
    set<QString> tmp;
    for(auto & e : pt)tmp.insert(e);
    for(int i = 0 ; i < tables.size() ; ++i){
        if(tmp.find(tables[i].getName()) != tmp.end()){
            v.push_back(tables[i]);
        }
    }
    return DataTable::CrossJoin(v);
}
