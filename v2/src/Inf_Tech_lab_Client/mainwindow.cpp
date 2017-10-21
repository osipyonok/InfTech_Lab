#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <databasesocket.h>
#include <QDebug>

MainWindow::MainWindow(DataBaseSocket *socket , QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->socket = socket;
    this->socket->doConnect("127.0.0.1");
    ui->setupUi(this);
    this->display = true;
    loadDataBases();
    getDataBasesTimer = new QTimer();
    connect(getDataBasesTimer , &QTimer::timeout , this , &MainWindow::loadDataBases);
    getDataBasesTimer->start(2000);

    connect(socket , &DataBaseSocket::gotData , this , &MainWindow::readyRead);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    QString dbName = last_entered;
    if(dbName.length() > 0){
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
            return;
        }
    }
    qDebug() << "some error " << last_entered << "\n";
}

void MainWindow::readyRead(QByteArray arr){
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
            auto viewer = new DataViewer(socket , name , login , pass , arr);
            connect(socket , &DataBaseSocket::gotData , viewer , &DataViewer::readyRead);
       //     disconnect(socket , &DataBaseSocket::gotData , this , &MainWindow::readyRead);
       //     this->getDataBasesTimer->stop();
            viewer->setAttribute(Qt::WA_DeleteOnClose);
            viewer->show();
       //     connect(socket , &DataBaseSocket::gotData , this , &MainWindow::readyRead);
       //     this->getDataBasesTimer->start(2000);
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
    qDebug() << "Yo\n";
    if(this->display == false)return;
    qDebug() << "Yo\n";
    QJsonObject obj;
    obj.insert("Command" , QJsonValue("getDataBases"));
    this->socket->write(QJsonDocument(obj).toJson());
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
    qDebug() << "kavo\n";
    if(createForm.getResult()){
        qDebug() << "kavo\n";
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
