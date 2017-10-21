#include "databaseconnection.h"

DataBaseConnection::DataBaseConnection(QSet<QString> *usedDBs)
{
    this->usedDBs = usedDBs;
}

QVector<QPair<QString , bool>> DataBaseConnection::getDataBases(){
    QDirIterator it(DATABASE_MAIN_DIRECTORY , QDirIterator::NoIteratorFlags);
    QVector<QPair<QString , bool>> res;
    while(it.hasNext()){
        it.next();
        auto info = it.fileInfo();
        if(info.isFile()){
            auto json = readJson(info.filePath());
            QString name = json.object()["name"].toString();
            res.push_back({name , usedDBs->find(name) != usedDBs->end()});
        }
    }
    return res;
}

QString DataBaseConnection::getDataBasePath(QString dbName){
    QDirIterator it(DATABASE_MAIN_DIRECTORY , QDirIterator::NoIteratorFlags);
    QVector<QPair<QString , bool>> res;
    while(it.hasNext()){
        it.next();
        auto info = it.fileInfo();
        if(info.isFile()){
            auto json = readJson(info.filePath());
            QString name = json.object()["name"].toString();
            if(name == dbName)return it.filePath();
        }
    }
    return "";
}

QByteArray DataBaseConnection::createDB(QString name , QString login , QString pass){
    QJsonObject tot;
    auto dbs = getDataBases();
    QJsonObject obj;
    qDebug() << "Ya tupa tut\n";
    if(name.length() == 0){
        obj.insert("Result" , QJsonValue(false));
        obj.insert("Error" , QJsonValue("Iм'я не може бути пустим"));
        qDebug() << "Ya tupa tut1\n";
    }else{
        for(auto it = dbs.begin() ; it != dbs.end() ; ++it){
            if((*it).first == name){
                QJsonObject obj;
                obj.insert("Result" , QJsonValue(false));
                obj.insert("Error" , QJsonValue("Введене iм'я вже використвується"));
                tot.insert("createDataBase" , QJsonValue(obj));
                qDebug() << "Ya tupa tut3\n";
                return QJsonDocument(tot).toJson();
            }
        }
        qDebug() << "Ya tupa tut2\n";
        DataBase db(name , login , pass);
        saveJson(db.toJson() , DATABASE_MAIN_DIRECTORY + rand_qstrq() + ".dblab");

        obj.insert("Result" , QJsonValue(true));
        obj.insert("Name" , QJsonValue(name));
        tot.insert("createDataBase" , QJsonValue(obj));
    }
    return QJsonDocument(tot).toJson();
}

QByteArray DataBaseConnection::openDB(QString name, QString login, QString pass){
    QJsonObject tot , obj;
    auto dbPath = getDataBasePath(name);
    if(dbPath.length() == 0){
        obj.insert("Result" , QJsonValue(false));
        obj.insert("Error" , QJsonValue("Базу даних не знайдено"));
    }else if(this->usedDBs->find(name) != this->usedDBs->end()){
        obj.insert("Result" , QJsonValue(false));
        obj.insert("Error" , QJsonValue("База даних вже використовується"));
    }else{
        this->db = new DataBase(readJson(dbPath).object());
        if(this->db->authentication(login , pass)){
            this->path = dbPath;
            this->usedDBs->insert(this->db->getName());
            obj.insert("Result" , QJsonValue(true));
            QJsonArray arr;
            for(auto e : this->db->getTableNamesSet()){
                arr.push_back(QJsonValue(e));
            }
            obj.insert("Tables" , QJsonValue(arr));
            obj.insert("Name" , QJsonValue(name));
            obj.insert("Login" , QJsonValue(login));
            obj.insert("Password" , QJsonValue(pass));
        }else{
            this->db = nullptr;
            this->path = "";
            obj.insert("Result" , QJsonValue(false));
            obj.insert("Error" , QJsonValue("Логiн та(або) пароль не вiрнi"));
        }
    }
    tot.insert("openDataBase" , QJsonValue(obj));
    return QJsonDocument(tot).toJson();
}

QByteArray DataBaseConnection::setMainData(QString name, QString login, QString pass){
    QJsonObject tot , obj;
    bool ck = true;
    if(name.length() > 0){
        ck &= db->setName(name);
        ck &= db->setLogin(login);
        ck &= db->setPassword(pass);
    }else{
        ck = false;
    }
    obj.insert("Result" , QJsonValue(ck));
    if(ck){
        obj.insert("Name" , QJsonValue(name));
        obj.insert("Login" , QJsonValue(login));
        obj.insert("Password" , QJsonValue(pass));
    }
    tot.insert("setMainData" , QJsonValue(obj));
    return QJsonDocument(tot).toJson();
}

QString DataBaseConnection::getDataBaseName(){
    return this->db->getName();
}

QByteArray DataBaseConnection::saveDataBase(){
    QJsonObject tot , obj;
    saveJson(this->db->toJson() , this->path);
    obj.insert("Result" , QJsonValue(true));
    tot.insert("saveDataBase" , QJsonValue(obj));
    return QJsonDocument(tot).toJson();
}

QByteArray DataBaseConnection::showTable(QString name){
    QJsonObject tot , obj;
    auto tablesNames = this->db->getTableNamesSet();
    bool ck = tablesNames.find(name) != tablesNames.end();
    obj.insert("Result" , QJsonValue(ck));
    if(ck){
        obj.insert("Table" , QJsonValue(this->db->getDataTable(name).toJson()));
    }
    tot.insert("showTable" , QJsonValue(obj));
    return QJsonDocument(tot).toJson();
}

QByteArray DataBaseConnection::deleteTable(QString name){
    QJsonObject tot , obj;
    bool ck = this->db->eraseTable(name);
    obj.insert("Result" , QJsonValue(ck));
    obj.insert("Name" , QJsonValue(name));
    tot.insert("deleteTable" , QJsonValue(obj));
    return QJsonDocument(tot).toJson();
}

QByteArray DataBaseConnection::saveTable(QString name, QJsonArray arr){
    QJsonObject tot , obj;
    map<pair<int , int> , QString> mp;
    for(QJsonValue e : arr){
        QJsonObject tmp = e.toObject();
        QString value = tmp.keys().at(0);
        QJsonObject pii = tmp[value].toObject();
        int fi = pii["First"].toInt();
        int se = pii["Second"].toInt();
        mp[make_pair(fi , se)] = value;
    }
    this->db->saveTable(name , mp);

    obj.insert("Result" , QJsonValue(true));
    tot.insert("saveTable" , QJsonValue(obj));
    return QJsonDocument(tot).toJson();
}

QByteArray DataBaseConnection::closeDataBase(){
    QJsonObject tot , obj;

    this->usedDBs->remove(this->db->getName());
    this->db = nullptr;
    this->path = "";

    obj.insert("Result" , QJsonValue(true));
    tot.insert("saveTable" , QJsonValue(obj));
    return QJsonDocument(tot).toJson();
}

QByteArray DataBaseConnection::deleteDataBase(){
    QJsonObject tot , obj;

    bool result = false;
    QDirIterator it(DATABASE_MAIN_DIRECTORY , QDirIterator::NoIteratorFlags);
    while(it.hasNext() && this->db != nullptr){
        it.next();
        auto info = it.fileInfo();
        if(info.isFile()){
            auto json = readJson(info.filePath());
            QString name = json.object()["name"].toString();
            if(name == this->db->getName()){
                result = QFile::remove(it.filePath());
                break;
            }
        }
    }

    this->usedDBs->remove(this->db->getName());
    this->db = nullptr;
    this->path = "";

    obj.insert("Result" , QJsonValue(result));
    tot.insert("deleteDataBase" , QJsonValue(obj));
    return QJsonDocument(tot).toJson();
}

QByteArray DataBaseConnection::addRow(QString name){
    QJsonObject tot , obj;

    auto desc = this->db->getDataTable(name).getDescription();
    vector<pair<int , QString>> v;
    for(int i = 0 ; i < desc.size() ; ++i){
        v.push_back(make_pair(desc[i].first , getDefaultValue(desc[i].second)));
    }
    this->db->addRow(v , name);

    obj.insert("Result" , QJsonValue(true));
    tot.insert("addRow" , QJsonValue(obj));
    return QJsonDocument(tot).toJson();
}

QByteArray DataBaseConnection::createTable(QString name , QJsonArray desc){
    QJsonObject tot , obj;

    vector<DataTypes> types;
    vector<QString> names;
    auto used_names = this->db->getTableNamesSet();
    if(used_names.find(name) == used_names.end()){
        for(QJsonValue val :  desc){
            QString key = val.toObject().keys().at(0);
            names.push_back(key);
            types.push_back(to_DataTypes(val.toObject()[key].toString()));
        }
        obj.insert("Result" , QJsonValue(this->db->createTable(name , types , names)));
        obj.insert("Name" , QJsonValue(name));
    }else{
        obj.insert("Result" , QJsonValue(false));
    }
    tot.insert("createTable" , QJsonValue(obj));

    return QJsonDocument(tot).toJson();
}
