#ifndef ADDITIONAL_FUNCTIONS_H
#define ADDITIONAL_FUNCTIONS_H
#include<bits/stdc++.h>
#include<QJsonDocument>
#include<QJsonObject>
#include<QFile>
#include<QString>
#include<QIODevice>

using namespace std;

namespace {
    void saveJson(QJsonDocument document, QString fileName) {
        QFile jsonFile(fileName);
        jsonFile.open(QFile::WriteOnly);
        jsonFile.write(document.toJson());
        jsonFile.close();
    }

    QString rand_qstrq(){
        QString s = "";
        for(int i = 0 ; i < 16 ; ++i){
            s += rand() % 26 + 'a';
        }
        return s;
    }

    QJsonDocument readJson(QString fileName){
        QFile jsonFile(fileName);
        jsonFile.open(QFile::ReadOnly | QFile::Text);
        QString content = jsonFile.readAll();
        jsonFile.close();
        QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8());
        if(doc.isNull() || doc.isEmpty()){
            cerr << "Json is null or empty" << endl;
        }
        return doc;
    }
}

#endif // ADDITIONAL_FUNCTIONS_H
