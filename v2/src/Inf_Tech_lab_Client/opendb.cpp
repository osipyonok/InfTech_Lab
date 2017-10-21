#include "opendb.h"
#include "ui_opendb.h"
#include <QDebug>

openDB::openDB(QString name , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::openDB)
{
    this->name = name;
    ui->setupUi(this);
    ui->titleLabel->setText("Введiть данi вiд " + this->name);
}

openDB::~openDB()
{
    delete ui;
}

void openDB::on_buttonBox_clicked(QAbstractButton *button)
{
    result = false;
    if(button->text() == "OK"){
        result = true;
        login = ui->loginEdit->text();
        pass = ui->passEdit->text();
    }
}

bool openDB::getResult()const{
    return result;
}

QString openDB::getLogin()const{
    return login;
}

QString openDB::getPassword()const{
    return pass;
}
