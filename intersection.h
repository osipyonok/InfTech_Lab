#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <QDialog>
#include <QTableWidget>
#include <QString>
#include <QCheckBox>
#include <database.h>

namespace Ui {
class Intersection;
}

class Intersection : public QDialog
{
    Q_OBJECT

public:
    explicit Intersection(QTableWidget *_table , DataBase *_db , QString _curTable , QWidget *parent = 0);
    ~Intersection();
    DataTable getResultTable()const;
    bool getResult()const;

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Intersection *ui;
    QTableWidget *table;
    DataBase *db;
    QString curTable;
    DataTable result_table;
    bool result;

};

#endif // INTERSECTION_H
