#include "projectiontable.h"
#include "ui_projectiontable.h"
#include <QLabel>
#include <QCheckBox>
#include <QDebug>
#include <QAbstractButton>

ProjectionTable::ProjectionTable(QTableWidget *table , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectionTable)
{
    tb = table;
    ui->setupUi(this);
    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
    for(int i = 0 ; i < tb->columnCount() ; ++i){
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        auto lab = new QLabel(ui->tableWidget);
        lab->setText(tb->horizontalHeaderItem(i)->text());
        ui->tableWidget->setCellWidget(ui->tableWidget->rowCount() - 1 , 0 , lab);
        auto cb = new QCheckBox(ui->tableWidget);
        cb->setChecked(true);
        ui->tableWidget->setCellWidget(ui->tableWidget->rowCount() - 1 , 1 , cb);
    }
}

ProjectionTable::~ProjectionTable()
{
    delete ui;
}

void ProjectionTable::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button->text() != "OK")return;
    for(int i = tb->columnCount() - 1  ; i >= 0 ; --i){
        if(qobject_cast<QCheckBox *>(ui->tableWidget->cellWidget(i , 1))->checkState() == Qt::Unchecked){
            qDebug() << "fddsfs\n";
            tb->setColumnHidden(i , true);
        }
    }
}
