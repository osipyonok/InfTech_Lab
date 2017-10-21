#ifndef PROJECTIONTABLE_H
#define PROJECTIONTABLE_H

#include <QDialog>
#include <QTableWidget>
#include <QAbstractButton>

namespace Ui {
class ProjectionTable;
}

class ProjectionTable : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectionTable(QTableWidget *table , QWidget *parent = 0);
    ~ProjectionTable();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::ProjectionTable *ui;
    QTableWidget *tb;
};

#endif // PROJECTIONTABLE_H
