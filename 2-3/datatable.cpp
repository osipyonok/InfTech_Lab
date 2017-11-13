#include "datatable.h"
#include<bits/stdc++.h>
#include<datatypes.h>
#include<datarow.h>
#include<additional_functions.h>
#include<QDebug>

using namespace std;

DataTable::DataTable(QString _name , vector<DataTypes> _description , vector<QString> _field_names)
{
    name = _name;
    int i = 0;
    for(auto d : _description){
        description.push_back({i , d});
        field_names.push_back({i , _field_names[i]});
        ++i;
    }
    is_description_sorted = false;
}

DataTable::DataTable(QJsonObject obj){
    if((obj.contains("name") && obj.contains("description") && obj.contains("rows")) == false){
        addMessage(Error , "Невiрний формат вхiдного файлу.");
        throw QString("Невiрний формат вхiдного файлу.");
    }

    name = obj.value("name").toString();

    QJsonObject jdesc = obj.value("description").toObject();

    for(auto key : jdesc.keys()){
        description.push_back({key.toInt() , to_DataTypes(jdesc.value(key).toString())});
    }

    QJsonObject jrows = obj.value("rows").toObject();

    for(auto e : jrows){
        rows.push_back(DataRow(e.toObject()));
    }

    QJsonObject jfieldnames = obj.value("field_names").toObject();

    for(auto key : jfieldnames.keys()){
        field_names.push_back({key.toInt() , jfieldnames.value(key).toString()});
    }


    is_description_sorted = false;
}

bool DataTable::operator==(DataTable & other){
    if(this->description.size() != other.description.size()){
        return false;
    }
    sortDescription();
    other.sortDescription();
    bool res = true;
    for(int i = 0 ; i < description.size() ; ++i){
        res &= (this->description[i].first == other.description[i].first);
        res &= (this->description[i].second == other.description[i].second);
    }
    return res;
}

void DataTable::sortDescription(){
    if(is_description_sorted)return;
    sort(description.begin() , description.end() ,
        [](const pair<int , DataTypes> & a, const pair<int , DataTypes> & b) -> bool
        {
            return a.first < b.first;
        }
    );
    is_description_sorted = true;
}

pair<bool , DataTypes> DataTable::getDataType(int index){
    if(is_description_sorted == false){
        sort(description.begin() , description.end() ,
            [](const pair<int , DataTypes> & a, const pair<int , DataTypes> & b) -> bool
            {
                return a.first < b.first;
            });
        is_description_sorted = true;
    }

    int l = 0 , r = description.size() - 1;
    while(r - l > 1){
        int mid = (r + l) / 2;
        if(description[mid].first > index){
            r = mid;
        }else{
            l = mid;
        }
    }

    if(description[r].first == index)return {true , description[r].second};
    if(description[l].first == index)return {true , description[l].second};
    return {false , Int};
}

bool DataTable::addRow(vector<pair<int , QString>> data){
    vector<DataTypes> used_datatypes;
    qDebug() << "allo qqq\n";
    if(validate(this , description , data , used_datatypes) == false){
    //    qDebug() << "Suck\n";
        return false;
    }

    rows.push_back(DataRow(used_datatypes , data));
    return true;
}

QJsonValue DataTable::toJson()
{
    QJsonObject obj{
        {"name" , QJsonValue(name)}
    };

    QJsonObject jrows;
    for(int i = 0 ; i < rows.size() ; ++i){
        jrows.insert(QString::number(i) , rows[i].toJson());
    }
    obj.insert("rows" , QJsonValue(jrows));

    QJsonObject jdesc;
    for(auto & e : description){
        jdesc.insert(QString::number(e.first) , to_string(e.second));
    }
    obj.insert("description" , QJsonValue(jdesc));

    QJsonObject jfieldnames;
    for(auto & e : field_names){
        jfieldnames.insert(QString::number(e.first) , e.second);
    }
    obj.insert("field_names" , QJsonValue(jfieldnames));

    QJsonValue val(obj);
    return obj;
}

bool validate(DataTable * _this , vector<pair<int , DataTypes>> & descr ,
                                vector<pair<int , QString>> & values , vector<DataTypes> & was){
    if(descr.size() < values.size()){
        return false;
    }

    bool result = true;
    for(auto e : values){
        int index = e.first;
        auto val = e.second;
        auto type = _this->getDataType(index);
        if(type.first == false){
            return false;
        }
        was.push_back(type.second);
        result &= is_parsable_as_type(type.second , val);
    }

    return result;
}

void DataTable::addMessage(MessageType type , QString mess){
    Messages::stack.append({type , mess});
}

QString DataTable::getName()const{
    return this->name;
}

vector<pair<int , DataTypes>> DataTable::getDescription()const{
    return this->description;
}

vector<pair<int , QString>> DataTable::getFieldNames()const{
    return this->field_names;
}

vector<DataRow> DataTable::getRows()const{
    return this->rows;
}

void DataTable::updateTable(map<pair<int , int> , QString> values){
    for(auto & e : values){
        rows[e.first.first].update(e.first.second , e.second);
    }
}

DataTable DataTable::Intersect(vector<DataTable> v){
    DataTable res;
    res.name = "Intersect";
    if(v.size() == 0)return res;
    res.description = v[0].description;
    res.field_names = v[0].field_names;
    vector<DataRow> rws = v[0].getRows();
    vector<bool> was(rws.size() , 1);
    for(int i = 0 ; i < rws.size() ; ++i){
        for(int j = 1 ; j < v.size() ; ++j){
            auto tmp = v[j].getRows();
            bool ck = 0;
            for(auto &o : tmp){
                if(o == rws[i]){
                    ck = 1;
                }
            }
            if(ck == 0)was[i] = 0;
        }
        if(was[i]){
            res.rows.push_back(rws[i]);
        }
    }
    return res;
}
