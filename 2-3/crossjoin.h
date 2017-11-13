#ifndef CROSSJOIN_H
#define CROSSJOIN_H

#include <QDialog>
#include <QTableWidget>
#include <QString>
#include <QCheckBox>
#include <database.h>

namespace Ui {
class CrossJoin;
}

class CrossJoin : public QDialog
{
    Q_OBJECT

public:
    explicit CrossJoin(QTableWidget *_table , DataBase *_db , QString _curTable , QWidget *parent = 0);
        ~CrossJoin();
        DataTable getResultTable()const;
        bool getResult()const;

private slots:
        void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::CrossJoin *ui;
    QTableWidget *table;
    DataBase *db;
    QString curTable;
    DataTable result_table;
    bool result;

};

#endif // CROSSJOIN_H
