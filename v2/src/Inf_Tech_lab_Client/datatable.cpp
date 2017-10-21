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
    qDebug() << "Hi from data table " << obj << "\n";
    if((obj.contains("name") && obj.contains("description") && obj.contains("rows")) == false){

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
    if(validate(this , description , data , used_datatypes) == false){
    //    qDebug() << "Suck\n";
        return false;
    }

    rows.push_back(DataRow(used_datatypes , data));
    return true;
}

bool DataTable::addRow(QJsonObject rowObj){
    this->rows.push_back(DataRow(rowObj));
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
    set<int> was;
    for(auto & e : values){
        rows[e.first.first].update(e.first.second , e.second);
        was.insert(e.first.first);
    }
    vector<DataRow> cur;
    for(int i = 0 ; i < rows.size() ; ++i){
        if(was.find(i) != was.end()){
            cur.push_back(rows[i]);
        }
    }
    rows = cur;
}

DataTable DataTable::CrossJoin(vector<DataTable> v){
    DataTable res;
    res.name = "CrossJoin";
    if(v.size() == 0)return res;
    res.description = v[0].description;
    res.field_names = v[0].field_names;
    int last_used_descr = -1;
    for(auto &e : res.description){
        last_used_descr = max(last_used_descr , e.first);
    }
    for(auto &e : res.field_names){
        e.second = v[0].getName() + "_" + e.second;
    }
    vector<DataRow> rws = v[0].getRows();
    for(int i = 1 ; i < v.size() ; ++i){
        vector<DataRow> cur;
        map<int , int> code;
        for(auto &e : v[i].getDescription()){
            code[e.first] = last_used_descr + 1;
            res.description.push_back({last_used_descr + 1 , e.second});
            ++last_used_descr;
        }
        for(auto &e : v[i].getFieldNames()){
            res.field_names.push_back({code[e.first] , v[i].getName() + "_" + e.second});
        }
        for(auto &row1 : rws){
            for(auto &row2 : v[i].getRows()){
                cur.push_back(DataRow::concat(row1, row2 , code));
            }
        }
        rws = cur;
    }
    res.rows = rws;
    return res;
}

DataTable DataTable::Project(DataTable table , vector<QString> _cols){
    DataTable res;
    res.name = "Projection";
    set<QString> cols;
    for(auto & e : _cols)cols.insert(e);
    sort(table.field_names.begin() , table.field_names.end());
    map<QString , int> mp;
    for(auto & e : table.field_names)mp[e.second] = e.first;
    set<int> need;
    for(auto & e : _cols){ if(mp.find(e) != mp.end())need.insert(mp[e]);}
    if(need.size() == 0)return res;
    for(DataRow & row : res.rows){
        res.rows.push_back(DataRow(row , need));
    }
    for(auto & e : _cols){
        if(mp.find(e) != mp.end())res.field_names.push_back({mp[e] , e});
    }
    table.sortDescription();
    auto desc = table.getDescription();
    for(auto & e : res.field_names){
        int idx = e.first;
        int l = 0  , r = desc.size();
        while(r - l > 1){
            int mid = (r + l) / 2;
            if(desc[mid].first > idx){
                r = mid;
            }else{
                l = mid;
            }
        }
        res.description.push_back(desc[l]);
    }
    return res;
}
