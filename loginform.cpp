#include "loginform.h"
#include "ui_loginform.h"
#include <QPushButton>
#include <QAbstractButton>

LoginForm::LoginForm(QJsonObject _obj , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    this->obj = _obj;
    this->name = obj.value("name").toString();
    this->login = obj.value("login").toString();
    this->pass = obj.value("pass").toString();

    ui->DB_Name_Label->setText("Авторизація в базі даних " + this->name);
}

LoginForm::~LoginForm()
{//dfdsfsd
    delete ui;
}

void LoginForm::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button->text() == "OK"){
        is_valid = ((login == ui->Login_Edit->text()) and (pass == ui->Pass_Edit->text()));
        qDebug() << "Valid: " << login << " " << pass << endl;
        qDebug() << "Inputed: " << ui->Login_Edit->text() << " " << ui->Pass_Edit->text() << endl;
    }
    this->close();
}

bool LoginForm::is_authenticated()const{
    return this->is_valid;
}

QJsonObject LoginForm::getObject()const{
    return this->obj;
}

QString LoginForm::getLogin()const{
    return this->login;
}

QString LoginForm::getPassword()const{
    return this->pass;
}
