        #include "databasethread.h"

    DataBaseThread::DataBaseThread(qintptr id , QSet<QString> *usedDBs , QObject *parent) :
        QThread(parent)
    {
        this->socketDescriptor = id;
        this->usedDBs = usedDBs;
        this->connection = new DataBaseConnection(this->usedDBs);
    }

    void DataBaseThread::run(){
        qDebug() << "Thread started\n";

        socket = new QTcpSocket();

        if(!socket->setSocketDescriptor(this->socketDescriptor)){
            emit error(socket->error());
            return;
        }

        connect(socket , SIGNAL(readyRead()) , this , SLOT(readyRead()) , Qt::DirectConnection);
        connect(socket , SIGNAL(disconnected()) , this , SLOT(disconnect()));

        exec();
    }

    void DataBaseThread::readyRead(){
        usedDBs->insert(QString::number(this->socketDescriptor));
        while(socket->canReadLine()){
            QByteArray data;
            int cnt = 0;
            do{
                QByteArray tmp = socket->readLine();
                for(auto e : tmp){
                    if((char)e == '{')++cnt;
                    if((char)e == '}')--cnt;
                    data.append(e);
                }
            }while(cnt != 0);
        //QByteArray data = socket->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QByteArray response;
        if(doc.object().contains("Command")){
            if(doc.object()["Command"].toString() == "getDataBases"){
                QJsonObject obj;
                for(auto &e : this->connection->getDataBases()){
                    obj.insert(e.first , QJsonValue(e.second));
                }
                QJsonObject tot;
                tot.insert("getDataBases" , obj);
                response = QJsonDocument(tot).toJson();
            }else if(doc.object()["Command"].toString() == "createDataBase"){
                QString Name = doc.object()["Params"].toObject()["Name"].toString();
                QString Login = doc.object()["Params"].toObject()["Login"].toString();
                QString Password = doc.object()["Params"].toObject()["Password"].toString();
                response = this->connection->createDB(Name , Login , Password);
            }else if(doc.object()["Command"].toString() == "openDataBase"){
                QString Name = doc.object()["Params"].toObject()["Name"].toString();
                QString Login = doc.object()["Params"].toObject()["Login"].toString();
                QString Password = doc.object()["Params"].toObject()["Password"].toString();
                response = this->connection->openDB(Name , Login , Password);
            }else if(doc.object()["Command"].toString() == "setMainData"){
                QString Name = doc.object()["Params"].toObject()["Name"].toString();
                QString Login = doc.object()["Params"].toObject()["Login"].toString();
                QString Password = doc.object()["Params"].toObject()["Password"].toString();
                response = this->connection->setMainData(Name , Login , Password);
            }else if(doc.object()["Command"].toString() == "saveDataBase"){
                response = this->connection->saveDataBase();
            }else if(doc.object()["Command"].toString() == "showTable"){
                QString name = doc.object()["Params"].toObject()["Name"].toString();
                response = this->connection->showTable(name);
            }else if(doc.object()["Command"].toString() == "deleteTable"){
                QString name = doc.object()["Params"].toObject()["Name"].toString();
                response = this->connection->deleteTable(name);
            }else if(doc.object()["Command"].toString() == "saveTable"){
                QString name = doc.object()["Params"].toObject()["Name"].toString();
                QJsonArray mp = doc.object()["Params"].toObject()["Map"].toArray();
                response = this->connection->saveTable(name , mp);
            }else if(doc.object()["Command"].toString() == "closeDataBase"){
                response = this->connection->closeDataBase();
            }else if(doc.object()["Command"].toString() == "addRow"){
                QString name = doc.object()["Params"].toObject()["Name"].toString();
                response = this->connection->addRow(name);
            }else if(doc.object()["Command"].toString() == "createTable"){
                QString name = doc.object()["Params"].toObject()["Name"].toString();
                QJsonArray desc = doc.object()["Params"].toObject()["Description"].toArray();
                response = this->connection->createTable(name , desc);
            }else if(doc.object()["Command"].toString() == "deleteDataBase"){
                response = this->connection->deleteDataBase();
            }else if(doc.object()["Command"].toString() == "getPossibleIntegsection"){
                QString tbl = doc.object()["Table"].toString();
                response = this->connection->getPossibleIntersectionTables(tbl);
            }else if(doc.object()["Command"].toString() == "makeIntersection"){
                QJsonArray arr = doc.object()["Tables"].toArray();
                response = this->connection->makeIntersection(arr);
            }
            socket->write(response);
        }else{
            socket->write(data);
        }
        qDebug() << socketDescriptor << " send " << data << "\n";}
    }

    void DataBaseThread::disconnect(){
        usedDBs->erase(usedDBs->find(QString::number(this->socketDescriptor)));
        qDebug() << socketDescriptor << " disconnected\n";
        socket->deleteLater();
        exit(0);
    }
