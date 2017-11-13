#include "crossjoin.h"
#include "ui_crossjoin.h"

CrossJoin::CrossJoin(QTableWidget *_table , DataBase *_db , QString _curTable , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CrossJoin)
{
    this->table = _table;
        this->db = _db;
        this->curTable = _curTable;
        ui->setupUi(this);
        auto table = this->db->getDataTable(curTable);

        ui->possibleTables->insertColumn(0);
        ui->possibleTables->insertColumn(1);
        ui->possibleTables->setHorizontalHeaderItem(0 , new QTableWidgetItem("Назва"));
        ui->possibleTables->setHorizontalHeaderItem(1 , new QTableWidgetItem("Вибiр"));
        for(auto &name : this->db->getTableNamesSet()){
            if(name != curTable){
                ui->possibleTables->insertRow(ui->possibleTables->rowCount());

                auto lab = new QLabel(ui->possibleTables);
                lab->setText(name);
                ui->possibleTables->setCellWidget(ui->possibleTables->rowCount() - 1 , 0 , lab);

                QWidget *cbWidget = new QWidget();
                auto cBox = new QCheckBox();
                QHBoxLayout *layoutCheckBox = new QHBoxLayout(cbWidget);
                layoutCheckBox->addWidget(cBox);
                layoutCheckBox->setAlignment(Qt::AlignCenter);
                layoutCheckBox->setContentsMargins(0 , 0 , 0 , 0);

                ui->possibleTables->setCellWidget(ui->possibleTables->rowCount() - 1 , 1 , cbWidget);
            }
        }
}

CrossJoin::~CrossJoin()
{
    delete ui;
}

void CrossJoin::on_buttonBox_clicked(QAbstractButton *button)
{
    result = false;
    if(button->text() == "OK"){
        qDebug() << button->text() << endl;
        vector<QString> v = {this->curTable};
        for(int i = 0 ; i < ui->possibleTables->rowCount() ; ++i){
            if(qobject_cast<QCheckBox *>(ui->possibleTables->cellWidget(i , 1)->
                                         layout()->itemAt(0)->widget())->checkState() == Qt::Checked){
                v.push_back(qobject_cast<QLabel *>(ui->possibleTables->cellWidget(i , 0))->text());
            }
        }
        this->result_table = this->db->CrossJoin(v);
        this->result = true;
    }else{
        this->result = false;
    }
}

DataTable CrossJoin::getResultTable()const{
    return this->result_table;
}

bool CrossJoin::getResult()const{
    return this->result;
}

