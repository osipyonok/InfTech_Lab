#ifndef CREATEDATABASE_H
#define CREATEDATABASE_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class CreateDataBase;
}

class CreateDataBase : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDataBase(QWidget *parent = 0);
    bool getResult();
    QString getName();
    QString getLogin();
    QString getPassword();
    ~CreateDataBase();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::CreateDataBase *ui;
    QString dbName;
    QString dbLogin;
    QString dbPass;
    bool res;
};

#endif // CREATEDATABASE_H
