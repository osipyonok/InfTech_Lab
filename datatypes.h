#ifndef DATATYPES_H
#define DATATYPES_H
#include<bits/stdc++.h>
#include<QJsonDocument>
#include<QJsonObject>
#include<QFile>
#include<QString>
#include<QRegExp>
#include<additional_functions.h>
#include "messagestack.h"
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
                qDebug() << "Rip int " << int_reg.exactMatch(value) << endl;
                return int_reg.exactMatch(value);
            }
            case Real:{
                QRegExp real_reg(R"(([+-]?[1-9]\d*|[+-]?0)(\.?([+-]?[1-9]\d*|[+-]?0))?)");
                qDebug() << "Rip real " << real_reg.exactMatch(value) << endl;
                return real_reg.exactMatch(value);
            }
            case Char:{
                QRegExp char_reg(".");
                qDebug() << "Rip char " << char_reg.exactMatch(value) << endl;
                return char_reg.exactMatch(value);
            }
            case String:{
            //    QRegExp string_reg(".+|(");
            //    return string_reg.exactMatch(value);
                return true;
            }
            case Range:{ // [begin;end{;step}]
                QRegExp range_reg(R"(\[(([+-]?[1-9]\d*|[+-]?0)(\.?([+-]?[1-9]\d*|[+-]?0))?)\;([+-]?[1-9]\d*|[+-]?0)(\.?([+-]?[1-9]\d*|[+-]?0))?(\;([+-]?[1-9]\d*|[+-]?0)(\.?([+-]?[1-9]\d*|[+-]?0))?)?\])");
                 qDebug() << "Rip range " << range_reg.exactMatch(value) << endl;
                return range_reg.exactMatch(value);
            }
            case Txtfile:{ // [file_name;context]
            //    QRegExp txtfile_reg(R"((\[[\w,\s-]+\.[A-Za-z]+\;(.|\n|\t)+\])|(\[\;\]))");
            //     qDebug() << "Rip txt " << txtfile_reg.exactMatch(value) << " " <<endl;
            //    return txtfile_reg.exactMatch(value);
                return true;
            }
        }
        return false;
    }

    QString trimBrackets(QString str){

    }

    QWidget * getWidget(QTableWidget *parent ,  DataTypes type , QString value){
        switch (type){
            case Int:{
                auto cur = new QSpinBox(parent);
                cur->setMinimum(INT_MIN);
                cur->setMaximum(INT_MAX);
                cur->setValue(value.toInt());
                return cur;
            }
            case Real:{
                auto cur = new QDoubleSpinBox(parent);
                cur->setMinimum(DBL_MIN);
                cur->setMaximum(DBL_MAX);
                cur->setValue(value.toDouble());
                return cur;
            }
            case Char:{
                auto cur = new QLineEdit(parent);
                cur->setText(value);
                cur->setMaxLength(1);
                return cur;
            }
            case String:{
                auto cur = new QLineEdit(parent);
                cur->setText(value);
                return cur;
            }
            case Range:{ // [begin;end{;step}]
                auto cur = new QWidget(parent);
        //        cur->resize(600,600);
                auto first_element = new QSpinBox(cur);
                auto last_element = new QSpinBox(cur);
                auto step = new QSpinBox(cur);

                first_element->setMinimum(INT_MIN);
                first_element->setMaximum(INT_MAX);
                last_element->setMinimum(INT_MIN);
                last_element->setMaximum(INT_MAX);
                step->setMinimum(INT_MIN);
                step->setMaximum(INT_MAX);
                //first_element->setp
                QStringList tmp = value.split(QRegExp(R"((\[|\]|\;))"));
                tmp.removeAll("");
                if(tmp.size() == 2){
                    tmp.append("1");
                }

         //       first_element->setValue(tmp.at(0).toInt());
                last_element->setValue(tmp.at(1).toInt());
                step->setValue(tmp.at(2).toInt());
                QHBoxLayout *layout = new QHBoxLayout();
        //        layout->setGeometry(QRect(0,0,500,500));
                layout->addWidget(first_element);
                layout->addWidget(last_element);
                layout->addWidget(step);
                cur->setLayout(layout);
                return cur;
            }/*
            case Txtfile:{ // [file_name;context]
                QRegExp txtfile_reg(R"(\[[\w,\s-]+\.[A-Za-z]+\;(.|\n|\t)+\])");
                return txtfile_reg.exactMatch(value);
            }*/
        }
        return new QLineEdit(parent);
    }
}


#endif // DATATYPES_H
