#include "createform.h"
#include "ui_createform.h"
#include <QMenu>
#include <QComboBox>
#include <datatypes.h>

CreateForm::CreateForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateForm)
{
    ui->setupUi(this);
    ui->colTable->insertColumn(0);
    ui->colTable->setHorizontalHeaderItem(0 , new QTableWidgetItem("Назва"));
    ui->colTable->insertColumn(1);
    ui->colTable->setHorizontalHeaderItem(1 , new QTableWidgetItem("Тип"));
    on_addCol_clicked();
}

CreateForm::~CreateForm()
{
    delete ui;
}

void CreateForm::on_addCol_clicked()
{
    ui->colTable->insertRow(ui->colTable->rowCount());
    ui->colTable->setCellWidget(ui->colTable->rowCount() - 1, 0, new QLineEdit(ui->colTable));
    auto box = new QComboBox();
    for(auto e : {Int , Real , Char , String , Range , Txtfile}){
        box->addItem(to_string(e));
    }
    ui->colTable->setCellWidget(ui->colTable->rowCount() - 1, 1, box);
    ui->colTable->resizeColumnsToContents();
    ui->colTable->resizeRowsToContents();
}

void CreateForm::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button->text() == "OK"){
        bool ck = 1;
        for(int i = 0 ; i < ui->colTable->rowCount() ; ++i){
            QString v1 = qobject_cast<QLineEdit *>(ui->colTable->cellWidget(i , 0))->text();
            QString v2 = qobject_cast<QComboBox *>(ui->colTable->cellWidget(i , 1))->currentText();
            rows.push_back({v1 , v2});
            ck &= v1.length() > 0;
        }
        name = ui->lineEdit->text();
        res = ck && name.length() > 0;
    }else{
        res = 0;
    }
 }


bool CreateForm::getResult(){
    return res;
}
vector<pair<QString , QString>> CreateForm::getRows(){
    return rows;
}

QString CreateForm::getName(){
    return name;
}

void CreateForm::on_deleteCol_clicked()
{
    if(ui->colTable->rowCount()){
        ui->colTable->removeRow(ui->colTable->rowCount() - 1);
    }
}
