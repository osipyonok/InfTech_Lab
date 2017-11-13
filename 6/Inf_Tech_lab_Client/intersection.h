#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <QDialog>
#include <QTableWidget>
#include <QString>
#include <QCheckBox>
#include <bits/stdc++.h>

using namespace std;


namespace Ui {
class Intersection;
}

class Intersection : public QDialog
{
    Q_OBJECT

public:
    explicit Intersection(QTableWidget *_table , vector<QString> possible ,  QString _curTable , QWidget *parent = 0);
    ~Intersection();
    vector<QString> getResultTables()const;
    bool getResult()const;

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Intersection *ui;
    QTableWidget *table;
    QString curTable;
    vector<QString> result_tables;
    bool result;

};

#endif // INTERSECTION_H
