#ifndef DATATYPES_H
#define DATATYPES_H
#include<bits/stdc++.h>
#include<QJsonDocument>
#include<QJsonObject>
#include<QFile>
#include<QString>
#include<QRegExp>
#include<additional_functions.h>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTableWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAbstractSpinBox>


using namespace std;

enum DataTypes {Int , Real , Char , String , Range , Txtfile};

namespace {
    vector<pair<QString , DataTypes>> DataTypesString = {
        {"Int" , Int} , {"Real" , Real} , {"Char" , Char} , {"String" , String} , {"Range" , Range} , {"Txtfile" , Txtfile}
    };

    vector<pair<QString , DataTypes>> DefaultValues = {
        {"0" , Int} , {"0.0" , Real} , {"A" , Char} , {"" , String} , {"[0;0;0]" , Range} , {"[;]" , Txtfile}
    };

    QString to_string(DataTypes type){
        for(auto & e : DataTypesString){
            if(e.second == type){
                return QString(e.first);
            }
        }
        return "String";
    }

    DataTypes to_DataTypes(QString s){
        for(auto & e : DataTypesString){
            if(e.first == s){
                return e.second;
            }
        }
        return String;
    }

    QString getDefaultValue(DataTypes type){
        for(auto & e : DefaultValues){
            if(e.second == type){
                return e.first;
            }
        }
    }

    bool is_parsable_as_type(const DataTypes & type , const QString & value){
        switch (type){
            case Int:{
                QRegExp int_reg(R"(([+-]?[1-9]\d*|[+-]?0))");
                return int_reg.exactMatch(value);
            }
            case Real:{
                QRegExp real_reg(R"([-+]?\d+(.\d+)?)");
                return real_reg.exactMatch(value);
            }
            case Char:{
                QRegExp char_reg(".");
                return char_reg.exactMatch(value);
            }
            case String:{
            //    QRegExp string_reg(".+|(");
            //    return string_reg.exactMatch(value);
                return true;
            }
            case Range:{ // [begin;end{;step}]
                QRegExp range_reg(R"(\[(([+-]?[1-9]\d*|[+-]?0)(\.?([+-]?[1-9]\d*|[+-]?0))?)\;([+-]?[1-9]\d*|[+-]?0)(\.?([+-]?[1-9]\d*|[+-]?0))?(\;([+-]?[1-9]\d*|[+-]?0)(\.?([+-]?[1-9]\d*|[+-]?0))?)?\])");
                return range_reg.exactMatch(value);
            }
            case Txtfile:{ // [file_name;context]
                if(value == "[;]")return true;
                QRegExp txtfile_reg(R"((\[[\w,\s-]+\.[A-Za-z]+\;(.|\n|\t)+\])|(\[\;\]))");
                return txtfile_reg.exactMatch(value);
            }
        }
        return false;
    }
}


#endif // DATATYPES_H
