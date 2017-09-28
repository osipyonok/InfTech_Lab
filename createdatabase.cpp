#include "createdatabase.h"
#include "ui_createdatabase.h"
#include <QAbstractButton>

CreateDataBase::CreateDataBase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDataBase)
{
    ui->setupUi(this);
}

CreateDataBase::~CreateDataBase()
{
    delete ui;
}

void CreateDataBase::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button->text() == "OK"){
        res = ui->dbName->text().length() > 0;
        dbName = ui->dbName->text();
        dbLogin = ui->dbLogin->text();
        dbPass = ui->dbPass->text();
    }
}

bool CreateDataBase::getResult(){
    return res;
}

QString CreateDataBase::getName(){
    return dbName;
}

QString CreateDataBase::getLogin(){
    return dbLogin;
}

QString CreateDataBase::getPassword(){
    return dbPass;
}
