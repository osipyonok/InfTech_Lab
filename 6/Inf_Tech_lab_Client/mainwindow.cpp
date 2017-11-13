#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <databasesocket.h>
#include <QDebug>

//#define CURL_STATICLIB
//#include <curl/curl.h>

MainWindow::MainWindow(DataBaseSocket *socket , HttpConnector *conn , QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ////////sign up client
    descriptor = rand_qstrq();
    auto pManager = new QNetworkAccessManager(this);
    QUrl serviceUrl = QUrl("http://dry-beach-27886.herokuapp.com/put");
    QByteArray postData;
    postData.append("author=" + descriptor + "&");
    postData.append("query=signup&");
    postData.append("time=" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    pManager->post(request , postData);

    WAIT_FOR_CONNECTION
    ////////
    this->socket = socket;
    this->socket->doConnect("127.0.0.1");//127.0.0.1
    ui->setupUi(this);
    this->display = true;
    loadDataBases();
    getDataBasesTimer = new QTimer();
    connect(getDataBasesTimer , &QTimer::timeout , this , &MainWindow::loadDataBases);
    getDataBasesTimer->start(2000);

    connect(socket , &DataBaseSocket::gotData , this , &MainWindow::readyRead);


    runCommandTimer = new QTimer();
    connect(runCommandTimer , &QTimer::timeout , this , &MainWindow::sendRequests);
    runCommandTimer->start(500);


    getResponseTimer = new QTimer();
    connect(getResponseTimer , &QTimer::timeout , this , &MainWindow::getResponse);
    getResponseTimer->start(500);
}


void MainWindow::getResponse(){
    auto pManager = new QNetworkAccessManager(this);
    QUrl serviceUrl = QUrl("http://dry-beach-27886.herokuapp.com/get?author=" + descriptor);
    connect(pManager , &QNetworkAccessManager::finished , this , &MainWindow::parseResponse);
    pManager->get(QNetworkRequest(serviceUrl));
}

void MainWindow::parseResponse(QNetworkReply *reply){
    auto response = reply->readAll();
    auto str_response = QString::fromUtf8(response);

    QStringList list;
    int cnt = 0;
    int last = 1;
    for(int i = 0 ; i < str_response.size() ; ++i){
        if(str_response[i] == '[')continue;
        else if(i == str_response.size() - 1){
            break;
        }else if(str_response[i] == '{'){
            ++cnt;
        }else if(str_response[i] == '}'){
            --cnt;
        }
        if(cnt == 0){
            list.append(str_response.mid(last , i - last + 1));
            last = i + 1;
        }
    }

    for(QString e : list){
    //    QJsonDocument doc = QJsonDocument::fromJson(e.replace("&#34;", "\"").toUtf8());
        auto arr = e.replace("&#34;", "\"").toUtf8();
        readyRead(arr);
        emit this->gotData(arr);
    }
}

void MainWindow::sendRequests(){
    while(!qRequests.empty()){
        auto data = qRequests.takeFirst();
        auto pManager = new QNetworkAccessManager(this);
        QUrl serviceUrl = QUrl("http://dry-beach-27886.herokuapp.com/put");
        QByteArray postData;
        postData.append("author=" + descriptor + "&");
        postData.append("query=" + data + "&");
        postData.append("time=" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        QNetworkRequest request(serviceUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        pManager->post(request , postData);
    }
}

void MainWindow::replyFinish(QNetworkReply* reply){
    QString answer = QString::fromUtf8(reply->readAll());
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    QString dbName = last_entered;
    if(dbName.length() > 0 && this->display){
        openDB opendb(dbName);
        opendb.exec();
        if(opendb.getResult()){
            QString login = opendb.getLogin();
            QString pass = opendb.getPassword();
            QJsonObject obj;
            obj.insert("Name" , QJsonValue(dbName));
            obj.insert("Login" , QJsonValue(login));
            obj.insert("Password" , QJsonValue(pass));
            QJsonObject tot;
            tot.insert("Command" , QJsonValue("openDataBase"));
            tot.insert("Params" , QJsonValue(obj));
            qDebug() << QJsonDocument(tot).toJson() << "\n";
            socket->write(QJsonDocument(tot).toJson());
            qRequests.append(QJsonDocument(tot).toJson());
            return;
        }
    }
    qDebug() << "some error " << last_entered << "\n";
}

void MainWindow::readyRead(QByteArray arr){
    qDebug() << "Got here " << arr << "\n";
    QJsonDocument doc = QJsonDocument::fromJson(arr);
    if(doc.object().contains("getDataBases")){
        QJsonObject obj = doc.object()["getDataBases"].toObject();
        QString selected = "";
        if(this->ui->dataBases->selectedItems().size()){
            selected = this->ui->dataBases->selectedItems().at(0)->text();
        }
        last_entered = selected;
        this->ui->dataBases->clear();
        for(auto e : obj.keys()){
            QListWidgetItem *item = new QListWidgetItem(this->ui->dataBases);
            item->setText(e);
            item->setSelected(selected.size() && selected == e);
            if(obj[e].toBool()){
                item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
            }
            this->ui->dataBases->addItem(item);
        }
    }else if(doc.object().contains("openDataBase")){
        QJsonObject obj = doc.object()["openDataBase"].toObject();
        if(obj.contains("Result") && obj["Result"].toBool()){
            QString name = obj["Name"].toString();
            QString login = obj["Login"].toString();
            QString pass = obj["Password"].toString();
            QJsonArray arr = obj["Tables"].toArray();
            auto viewer = new DataViewer(&this->qRequests ,socket , name , login , pass , arr);
            connect(socket , &DataBaseSocket::gotData , viewer , &DataViewer::readyRead);
            connect(this , &MainWindow::gotData , viewer , &DataViewer::readyRead);
            viewer->setAttribute(Qt::WA_DeleteOnClose);
            viewer->show();
        }else{
            QMessageBox error;
            error.setWindowTitle("Помилка");
            error.setText("Невірний логін або пароль.");
            error.setIcon(QMessageBox::Critical);
            error.exec();
        }
    }
}

void MainWindow::loadDataBases(){
    if(this->display == false)return;
    QJsonObject obj;
    obj.insert("Command" , QJsonValue("getDataBases"));
    this->socket->write(QJsonDocument(obj).toJson());
    qRequests.append(QJsonDocument(obj).toJson());
}

void MainWindow::on_dataBases_entered(const QModelIndex index)
{
    last_entered = index.data().toString();
    //last_entered = this->ui->dataBases->itemAt(index.row() , index.column())->text();
}

void MainWindow::on_createDB_clicked()
{
    CreateDataBase createForm;
    createForm.exec();
    if(createForm.getResult()){
        QJsonObject obj;
        obj.insert("Name" , QJsonValue(createForm.getName()));
        obj.insert("Login" , QJsonValue(createForm.getLogin()));
        obj.insert("Password" , QJsonValue(createForm.getPassword()));
        QJsonObject tot;
        tot.insert("Command" , QJsonValue("createDataBase"));
        tot.insert("Params" , QJsonValue(obj));
        this->socket->write(QJsonDocument(tot).toJson());
    }
}

void MainWindow::on_dataBases_clicked(const QModelIndex index)
{
    this->last_entered = index.data().toString();
}

void MainWindow::on_dataBases_doubleClicked(const QModelIndex index)
{
    this->last_entered = index.data().toString();
    on_connectButton_clicked();
}
