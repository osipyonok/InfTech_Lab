#include "dataviewer.h"
#include "ui_dataviewer.h"
#include <QStringListModel>
#include <additional_functions.h>
#include <QFileDialog>
#include <QDir>
#include <projectiontable.h>

DataViewer::DataViewer(QJsonObject obj , QString path , QString login , QString pass , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataViewer)
{
    this->db = DataBase(obj);
    this->db.authentication(login , pass);
    this->dbPath = path;
    ui->setupUi(this);
    QStringList *stringList = new QStringList();
    stringList->append("Головна панель");
    for(QString e : this->db.getTableNamesSet()){
        stringList->append(e);
    }
    auto listModel = new QStringListModel(*stringList, NULL);
    ui->tablesList->setModel(listModel);
    ui->tablesList->setAcceptDrops(false);
    ui->tablesList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tablesList->setCurrentIndex(listModel->index(0, 0));
    ui->dbNameLabel->setText("База даних " + this->db.getName());
    loadDataToEdit();
    ShowTable(SHOW_MAIN_PANEL);
    this->setWindowTitle("База даних " + this->db.getName());
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    //QWidget::adjustSize();

}

DataViewer::DataViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataViewer)
{
    ui->setupUi(this);
}

DataViewer::~DataViewer()
{
    delete ui;
}

void DataViewer::on_tablesList_doubleClicked(const QModelIndex &index)
{
    auto cur = index.data().toString();
    if(cur == "Головна панель" and index.row() == 0){
        cur = SHOW_MAIN_PANEL;
    }
    ShowTable(cur);
}

void DataViewer::ShowTable(DataTable table){
    ui->widget->hide();
    QDir dir("temp\\");
    dir.removeRecursively();
    auto description = table.getDescription();
    sort(description.begin() , description.end() ,
        [](const pair<int , DataTypes> & a, const pair<int , DataTypes> & b) -> bool
        {
            return a.first < b.first;
        });
    auto fields = table.getFieldNames();
    sort(fields.begin() , fields.end());
    ui->dbTable->setColumnCount(0);
    ui->dbTable->setRowCount(0);
    for(int i = 0 ; i < fields.size() ; ++i){
        int p = fields[i].first;
        ui->dbTable->insertColumn(p);
        ui->dbTable->setHorizontalHeaderItem(p , new QTableWidgetItem(fields[i].second));
    }
    auto rows = table.getRows();
    for(int i = 0 ; i < rows.size() ; ++i){
        auto &e = rows[i];
        ui->dbTable->insertRow(i);
        auto data = e.getData();
        int j = 0;
        for(auto &o : data){
            ui->dbTable->setCellWidget(i , o.first , new DataValueWidget(o.second.type , o.second.value , i , j++ , ui->dbTable));
        }
    }
    ui->dbTable->resizeColumnsToContents();
    ui->dbTable->resizeRowsToContents();
}

void DataViewer::ShowTable(QString name){
    if(name == SHOW_MAIN_PANEL){
        ui->widget->show();
        ui->tableWidget->hide();
        ui->deleteTableButton->setVisible(false);
        ui->deleteRowButton->setVisible(false);
        ui->addRowButton->setVisible(false);
        ui->makeProjection->setVisible(false);
        ui->IntersectionButton->setVisible(false);
    }else{
        DataTable table = this->db.getDataTable(name);
        ShowTable(table);
        this->last_table = name;
        ui->deleteTableButton->setVisible(true);
        ui->deleteRowButton->setVisible(true);
        ui->addRowButton->setVisible(true);
    //    ui->makeProjection->setVisible(true);
        ui->IntersectionButton->setVisible(true);
        ui->tableWidget->show();
    }
}

void DataViewer::on_displayPassword_stateChanged(int arg1)
{
    if(ui->dbPassEdit->echoMode() == QLineEdit::Normal){
        ui->dbPassEdit->setEchoMode(QLineEdit::Password);
    }else{
        ui->dbPassEdit->setEchoMode(QLineEdit::Normal);
    }
}

void DataViewer::loadDataToEdit(){
    ui->dbNameEdit->setText(this->db.getName());
    ui->dbLoginEdit->setText(this->db.getLogin());
    ui->dbPassEdit->setText(this->db.getPassword());
}

void DataViewer::on_dbEditReloadButton_clicked()
{
    loadDataToEdit();
}

void DataViewer::on_dbEditSaveButton_clicked()
{
    this->db.setName(ui->dbNameEdit->text());
    this->db.setLogin(ui->dbLoginEdit->text());
    this->db.setPassword(ui->dbPassEdit->text());
    ui->dbNameLabel->setText("База даних " + this->db.getName());
    this->setWindowTitle("База даних " + this->db.getName());
}

void DataViewer::on_dbSaveButton_clicked()
{
    if(this->dbPath.length() == 0){
        on_dbSaveAsButton_clicked();
        return;
    }
    saveJson(this->db.toJson(), this->dbPath);
}

void DataViewer::on_dbSaveAsButton_clicked()
{
    QFileDialog file;
    file.setAcceptMode(QFileDialog::AcceptSave);
    file.setNameFilter(tr("Inf tech lab database (*.dblab)"));
    file.exec();
    if(file.selectedFiles().size() == 0){
        return;
    }
    QString path = file.selectedFiles().last();
    saveJson(this->db.toJson(), path);
    this->dbPath = path;
}

void DataViewer::on_dbSaveTable_clicked()
{
    map<pair<int , int> , QString> mp;

    for(int i = 0 ; i < ui->dbTable->rowCount() ; ++i){
        for(int j = 0 ; j < ui->dbTable->columnCount() ; ++j){
            auto e = qobject_cast<DataValueWidget *>(ui->dbTable->cellWidget(i , j));
            mp[make_pair(e->getRow() , e->getCol())] = e->getValue();
        }
    }
    this->db.saveTable(last_table , mp);
}

void DataViewer::on_dbExitButton_clicked()
{
    this->close();
}

void DataViewer::on_deleteTableButton_clicked()
{
    if(this->db.eraseTable(last_table)){
        QStringList *stringList = new QStringList();
        stringList->append("Головна панель");
        for(QString e : this->db.getTableNamesSet()){
            stringList->append(e);
        }
        auto listModel = new QStringListModel(*stringList, NULL);
        ui->tablesList->setModel(listModel);
        ui->tablesList->setAcceptDrops(false);
        ui->tablesList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tablesList->setCurrentIndex(listModel->index(0, 0));
        loadDataToEdit();
        ShowTable(SHOW_MAIN_PANEL);
    }
}

void DataViewer::on_dbCreateTable_clicked()
{
    CreateForm createForm;
    createForm.exec();
    if(createForm.getResult()){
        vector<DataTypes> types;
        vector<QString> names;
        auto tmp = createForm.getRows();
        for(auto &e : tmp){
            names.push_back(e.first);
            types.push_back(to_DataTypes(e.second));
        }
        this->db.createTable(createForm.getName() , types , names);
        QStringList *stringList = new QStringList();
        stringList->append("Головна панель");
        for(QString e : this->db.getTableNamesSet()){
            stringList->append(e);
        }
        auto listModel = new QStringListModel(*stringList, NULL);
        ui->tablesList->setModel(listModel);
        ui->tablesList->setAcceptDrops(false);
        ui->tablesList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tablesList->setCurrentIndex(listModel->index(0, 0));
     //   ShowTable(createForm.getName());
    }
}

void DataViewer::on_deleteRowButton_clicked()
{
    for(int i = ui->dbTable->selectionModel()->selectedRows().length() - 1 ; i >= 0 ; --i){
    //    qDebug() << ui->dbTable->selectionModel()->selectedRows().at(i). << endl;
        ui->dbTable->removeRow(ui->dbTable->selectionModel()->selectedRows().at(i).row());
    }
}

void DataViewer::on_addRowButton_clicked()
{
    qDebug() << "dgfdgdf\n";
    auto desc = this->db.getDataTable(last_table).getDescription();
    vector<pair<int , QString>> v;
    for(int i = 0 ; i < desc.size() ; ++i){
        v.push_back(make_pair(desc[i].first , getDefaultValue(desc[i].second)));
        qDebug() << v[i].first << " " << v[i].second << endl;
    }
//    qDebug() << "Size - " << this->db.tables.size() << endl;
    this->db.addRow(v , last_table);
    ShowTable(last_table);

//    qDebug() << this->db.tables.size() << endl;
}

void DataViewer::on_makeProjection_clicked()
{
    ShowTable(last_table);
    ProjectionTable pt(ui->dbTable);
    pt.exec();
}

void DataViewer::on_IntersectionButton_clicked()
{
    ShowTable(last_table);
    Intersection intersection(ui->dbTable , &this->db , last_table);
    intersection.exec();
    if(intersection.getResult()){
        qDebug() << "retretetete\n";
        ShowTable(intersection.getResultTable());
    }
}
