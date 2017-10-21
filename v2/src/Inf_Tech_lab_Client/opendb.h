#ifndef OPENDB_H
#define OPENDB_H

#include <QDialog>
#include <QAbstractButton>
#include <QString>

namespace Ui {
class openDB;
}

class openDB : public QDialog
{
    Q_OBJECT

public:
    explicit openDB(QString name , QWidget *parent = 0);
    ~openDB();
    bool getResult()const;
    QString getLogin()const;
    QString getPassword()const;

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::openDB *ui;
    QString name;
    QString login;
    QString pass;
    bool result;
};

#endif // OPENDB_H
