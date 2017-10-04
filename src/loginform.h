#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include<bits/stdc++.h>
#include<QJsonDocument>
#include<QJsonObject>
#include<QFile>
#include<QString>
#include<QRegExp>
#include<additional_functions.h>
#include "messagestack.h"
#include <QAbstractButton>

namespace Ui {
class LoginForm;
}

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(QJsonObject _obj , QWidget *parent = 0);
    bool is_authenticated()const;
    QJsonObject getObject()const;
    QString getLogin()const;
    QString getPassword()const;
    ~LoginForm();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::LoginForm *ui;
    QString name;
    QString login , pass;
    QJsonObject obj;
    bool is_valid = false;
};

#endif // LOGINFORM_H
