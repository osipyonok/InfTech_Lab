#include "iostream"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "database.h"
#include <datatypes.h>
#include<additional_functions.h>
#include<QRegExp>
#include<loginform.h>
#include<QFileDialog>
#include<QFile>
#include<QMessageBox>
#include<dataviewer.h>
#include <QWidget>
#include <createdatabase.h>

using namespace std;

void run();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
 //   run();
    ui->setupUi(this);
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);

}

void run(){//DEBUG //https://creately.com/app/?tempID=h165rwt81&login_type=demo#
//    DataBase db("3.txt");
    DataBase db("test_db" , "login" , "pass");

    db.createTable("NewTable" , {Int , String} , {"id" , "name"});
//    QJsonDocument doc = readJson("3.txt");
//    qDebug() << QString(doc.toJson()) << endl;
    db.addRow({{0 , "-12"} , {1 , "ee"}});
    db.addRow({{0 , "52"} , {1 , "azazaza"}});
    db.createTable("AnotherTable" , {Int , Int , Char} , {"ID" , "Year" , "char_value"});
    db.addRow({{0 , "1488"} , {1 , "228"} , {2 , "A"}});
 //   cerr << db.tables[db.active_table].rows.size() << endl;

    saveJson(db.toJson() , "another.txt");
 //   saveJson(db.tables[db.active_table].rows[0].toJson() , "1.txt");

}

MainWindow::~MainWindow()
{
    QDir dir("temp\\");
    dir.removeRecursively();
    delete ui;
}

void MainWindow::on_Close_button_clicked()
{
    this->close();
}

void MainWindow::on_OpenDB_button_clicked()
{
    QFileDialog file;
    file.setFileMode(QFileDialog::ExistingFile);
    file.setNameFilter(tr("Inf tech lab database (*.dblab)"));
    file.exec();
    if(file.selectedFiles().size() == 0){
        return;
    }
    QString path = file.selectedFiles().last();
    QJsonDocument doc = readJson(path);
    if(!path.length() or doc.isNull() or doc.isEmpty() or !doc.object().keys().contains("name")){
        QMessageBox error;
        error.setWindowTitle("Помилка");
        error.setText("Не вдалося прочитати файл.");
        error.setIcon(QMessageBox::Critical);
        error.exec();
        return;
    }
    LoginForm login_form(doc.object());
    login_form.exec();
    if(login_form.is_authenticated()){
        DataViewer dataviewer(login_form.getObject(), path , login_form.getLogin(), login_form.getPassword());
        this->close();
        dataviewer.exec();
        this->show();
    }else{
        QMessageBox error;
        error.setWindowTitle("Помилка");
        error.setText("Невірний логін або пароль.");
        error.setIcon(QMessageBox::Critical);
        error.exec();
    }
}

void MainWindow::on_CreateDB_button_clicked()
{
    CreateDataBase f;
    f.exec();
    if(f.getResult()){
        DataBase db(f.getName() , f.getLogin() , f.getPassword());
        DataViewer dw(db.toJson().object() , "" , f.getLogin() , f.getPassword());
        this->close();
        dw.exec();
        this->show();
    }
}
