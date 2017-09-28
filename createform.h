#ifndef CREATEFORM_H
#define CREATEFORM_H

#include <QDialog>
#include <QHBoxLayout>
#include <QAbstractButton>
#include <bits/stdc++.h>

using namespace std;

namespace Ui {
class CreateForm;
}

class CreateForm : public QDialog
{
    Q_OBJECT

public:
    explicit CreateForm(QWidget *parent = 0);
    ~CreateForm();
    bool getResult();
    vector<pair<QString , QString>> getRows();
    QString getName();

private slots:
    void on_addCol_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_deleteCol_clicked();

private:
    Ui::CreateForm *ui;
    bool res;
    vector<pair<QString , QString>> rows;
    QString name = "";
};

#endif // CREATEFORM_H
