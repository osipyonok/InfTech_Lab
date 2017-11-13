#include "dataviewer.h"
#include "ui_dataviewer.h"
#include <QStringListModel>
#include <QFileDialog>
#include <QDir>
#include <QQueue>
#include <intersection.h>


DataViewer::DataViewer(QQueue<QByteArray> *q, DataBaseSocket *socket , QString name , QString login ,
                       QString pass , QJsonArray tables , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataViewer)
{
    this->qRequests = q;
    this->socket = socket;
    this->dbName = name;
    this->login = login;
    this->password = pass;
    this->tables = tables;
    ui->setupUi(this);
    QStringList *stringList = new QStringList();
    stringList->append("Головна панель");
    for(auto val : tables){
        stringList->append(val.toString());
    }
    auto listModel = new QStringListModel(*stringList, NULL);
    ui->tablesList->setModel(listModel);
    ui->tablesList->setAcceptDrops(false);
    ui->tablesList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tablesList->setCurrentIndex(listModel->index(0, 0));
    ui->dbNameLabel->setText("База даних " + name);
    loadDataToEdit();
    ShowTable(SHOW_MAIN_PANEL);
    this->setWindowTitle("База даних " + name);
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
    this->cache[last_table] = openedTable;
    ShowTable(cur);
}

void DataViewer::ShowTable(QJsonObject table){
    DataTable dataTable(table);
    ShowTable(dataTable);
    this->ui->tableWidget->show();
}

void DataViewer::reject(){
    {
        QJsonObject obj;
        obj.insert("Command" , QJsonValue("getDataBases"));
        qRequests->append(QJsonDocument(obj).toJson());
    }
    QJsonObject tot;
    tot.insert("Command" , "closeDataBase");
    socket->write(QJsonDocument(tot).toJson());
    qRequests->append(QJsonDocument(tot).toJson());
    QDialog::reject();
}

void DataViewer::ShowTable(QString name){
    qDebug() << "in swow table string\n";
    this->openedTable.updateTable(getMapForSave());
    if(name == SHOW_MAIN_PANEL){
        cache[this->openedTable.getName()] = this->openedTable;
        ui->widget->show();
        ui->tableWidget->hide();
        ui->deleteTableButton->setVisible(false);
        ui->deleteRowButton->setVisible(false);
        ui->addRowButton->setVisible(false);
        ui->makeProjection->setVisible(false);
    }else{
        cache.clear();//temp bug fix
        if(cache.find(name) != cache.end()){
            qDebug() << "Entered to CACHE\n";
            cache[this->openedTable.getName()] = this->openedTable;
            ShowTable(cache[name]);
            ui->tableWidget->setVisible(true);
            return;
        }

        QJsonObject tot , obj;
        obj.insert("Name" , QJsonValue(name));
        tot.insert("Command" , QJsonValue("showTable"));
        tot.insert("Params" , QJsonValue(obj));
        socket->write(QJsonDocument(tot).toJson());
        qRequests->append(QJsonDocument(tot).toJson());
    //    ui->tableWidget->show();
    }
}

void DataViewer::ShowTable(DataTable table){

    this->last_table = table.getName();
    ui->deleteTableButton->setVisible(true);
    ui->deleteRowButton->setVisible(true);
    ui->addRowButton->setVisible(true);
    ui->makeProjection->setVisible(true);
    openedTable = table;
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

void DataViewer::on_displayPassword_stateChanged(int arg1)
{
    if(ui->dbPassEdit->echoMode() == QLineEdit::Normal){
        ui->dbPassEdit->setEchoMode(QLineEdit::Password);
    }else{
        ui->dbPassEdit->setEchoMode(QLineEdit::Normal);
    }
}

void DataViewer::loadDataToEdit(){
    ui->dbNameEdit->setText(this->dbName);
    ui->dbLoginEdit->setText(this->login);
    ui->dbPassEdit->setText(this->password);
}

void DataViewer::on_dbEditReloadButton_clicked()
{
    loadDataToEdit();
}

void DataViewer::on_dbEditSaveButton_clicked()
{
    QJsonObject tot , obj;

    obj.insert("Name" , QJsonValue(ui->dbNameEdit->text()));
    obj.insert("Login" , QJsonValue(ui->dbLoginEdit->text()));
    obj.insert("Password" , QJsonValue(ui->dbPassEdit->text()));
    tot.insert("Command" , QJsonValue("setMainData"));
    tot.insert("Params" , QJsonValue(obj));
    socket->write(QJsonDocument(tot).toJson());
    qRequests->append(QJsonDocument(tot).toJson());
}

void DataViewer::readyRead(QByteArray arr){
    qDebug() << "READY READ " << arr << "\n";
    QJsonDocument doc = QJsonDocument::fromJson(arr);
    if(doc.object().contains("setMainData")){
        if(doc.object()["setMainData"].toObject()["Result"].toBool()){
            dbName = doc.object()["setMainData"].toObject()["Name"].toString();
            login = doc.object()["setMainData"].toObject()["Login"].toString();
            password = doc.object()["setMainData"].toObject()["Password"].toString();

            ui->dbNameLabel->setText("База даних " + dbName);
            this->setWindowTitle("База даних " + dbName);
        }
    }else if(doc.object().contains("showTable")){
    /*    qDebug() << "In show table\n";
       // qDebug() << doc.object()["showTable"].toObject()["Table"].toObject() << "\n";
       // qDebug() << doc.object()["showTable"].toObject()["Result"].toBool() << "\n";
       // return;
        if(doc.object()["showTable"].toObject()["Result"].toBool()){
            qDebug() << "In this stuff\n";
         //   QJsonObject table = doc.object()["showTable"].toObject()["Table"].toObject();
            QString str = doc.object()["showTable"].toObject()["Table"].toString();
         //   qDebug() << str << "\n";
         //   qDebug() << doc.object()["showTable"].toObject() << "\n";
         //   qDebug() << doc.object()["showTable"].toObject()["Result"].toString() << "\n";
         //   return;
            ShowTable(QJsonDocument::fromJson(str.toUtf8()).object());
        }else{
            ShowTable(SHOW_MAIN_PANEL);
        }*/

                if(doc.object()["showTable"].toObject()["Result"].toBool()){
                    QJsonObject table = doc.object()["showTable"].toObject()["Table"].toObject();
                    qDebug() << "Show table got the following:" << table ;
                    ShowTable(table);
                }else{
                    ShowTable(SHOW_MAIN_PANEL);
                }
    }else if(doc.object().contains("deleteTable")){
        if(doc.object()["deleteTable"].toObject()["Result"].toBool()){
            QStringList *stringList = new QStringList();
            stringList->append("Головна панель");
            QJsonArray arr;
            for(auto e : tables){
                if(e.toString() != doc.object()["deleteTable"].toObject()["Name"].toString()){
                    arr.push_back(e);
                }
            }
            tables = arr;
            for(QJsonValue e : tables){
                stringList->append(e.toString());
            }
            auto listModel = new QStringListModel(*stringList, NULL);
            ui->tablesList->setModel(listModel);
            ui->tablesList->setAcceptDrops(false);
            ui->tablesList->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->tablesList->setCurrentIndex(listModel->index(0, 0));
            loadDataToEdit();
            ShowTable(SHOW_MAIN_PANEL);
        }
    }else if(doc.object().contains("addRow")){
        if(doc.object()["addRow"].toObject()["Result"].toBool()){
            cache.erase(last_table);
            ShowTable(last_table);
        }
    }else if(doc.object().contains("createTable")){
        if(doc.object()["createTable"].toObject()["Result"].toBool()){
            QStringList *stringList = new QStringList();
            stringList->append("Головна панель");
            QString name = doc.object()["createTable"].toObject()["Name"].toString();
            tables.push_back(name);
            sort(stringList->begin() + 1 , stringList->end());
            for(QJsonValue e : tables){
                stringList->append(e.toString());
            }
            auto listModel = new QStringListModel(*stringList, NULL);
            ui->tablesList->setModel(listModel);
            ui->tablesList->setAcceptDrops(false);
            ui->tablesList->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->tablesList->setCurrentIndex(listModel->index(0, 0));
            ShowTable(SHOW_MAIN_PANEL);
        }
    }else if(doc.object().contains("getPossibleIntegsection")){
        if(doc.object()["getPossibleIntegsection"].toObject()["Result"].toBool()){
            QJsonArray arr = doc.object()["getPossibleIntegsection"].toObject()["Tables"].toArray();
            vector<QString> v;
            for(QJsonValue val : arr)
                v.push_back(val.toString());
            Intersection intersection(ui->dbTable , v , last_table);
            intersection.exec();
            if(intersection.getResult()){
                QJsonArray eee;
                for(auto s : intersection.getResultTables())eee.append(s);
                QJsonObject tot;
                tot.insert("Command" , QJsonValue("makeIntersection"));
                tot.insert("Tables" , QJsonValue(eee));
                socket->write(QJsonDocument(tot).toJson());
                //qRequests->append(QJsonDocument(tot).toJson());
                //qDebug() << intersection.getResultTables();
            }
        }
    }else if(doc.object().contains("makeIntersection")){
        qDebug() << "NIGGA YO\n";
        if(doc.object()["makeIntersection"].toObject()["Result"].toBool()){
            QJsonObject tbl = doc.object()["makeIntersection"].toObject()["Table"].toObject();
            ShowTable(tbl);
        }
    }

}

void DataViewer::on_dbSaveButton_clicked()
{
    QJsonObject tot;
    tot.insert("Command" , "saveDataBase");
    socket->write(QJsonDocument(tot).toJson());
    qRequests->append(QJsonDocument(tot).toJson());
}

map<pair<int , int> , QString> DataViewer::getMapForSave(){
    map<pair<int , int> , QString> mp;

    for(int i = 0 ; i < ui->dbTable->rowCount() ; ++i) {
        for(int j = 0 ; j < ui->dbTable->columnCount() ; ++j){
            auto e = qobject_cast<DataValueWidget *>(ui->dbTable->cellWidget(i , j));
            mp[make_pair(e->getRow() , e->getCol())] = e->getValue();
        }
    }

    return mp;
}

void DataViewer::on_dbSaveTable_clicked()
{
    map<pair<int , int> , QString> mp = getMapForSave();
    this->openedTable.updateTable(mp);
    cache[last_table] = this->openedTable;

    QJsonObject tot , obj;
    QJsonArray arr;
    for(auto e : mp){
        QJsonObject cur , tmp;
        tmp.insert("First" , QJsonValue(e.first.first));
        tmp.insert("Second" , QJsonValue(e.first.second));
        cur.insert(e.second , QJsonValue(tmp));
        arr.push_back(QJsonValue(cur));
    }
    obj.insert("Map" , QJsonValue(arr));
    obj.insert("Name" , QJsonValue(last_table));
    tot.insert("Command" , QJsonValue("saveTable"));
    tot.insert("Params" , QJsonValue(obj));
    qDebug() << "savetable " << tot << "\n";
    socket->write(QJsonDocument(tot).toJson());
    qRequests->append(QJsonDocument(tot).toJson());
}

void DataViewer::on_dbExitButton_clicked()
{
    this->reject();
}

void DataViewer::on_deleteTableButton_clicked()
{
    QJsonObject tot , obj;
    obj.insert("Name" , this->last_table);
    tot.insert("Command" , QJsonValue("deleteTable"));
    tot.insert("Params" , QJsonValue(obj));
    socket->write(QJsonDocument(tot).toJson());
    qRequests->append(QJsonDocument(tot).toJson());
}

void DataViewer::on_dbCreateTable_clicked()
{
    CreateForm createForm;
    createForm.exec();
    if(createForm.getResult()){
        vector<DataTypes> types;
        vector<QString> names;
        QString name = createForm.getName();
        auto tmp = createForm.getRows();
        for(auto &e : tmp){
            names.push_back(e.first);
            types.push_back(to_DataTypes(e.second));
        }
        QJsonObject tot , obj;
        obj.insert("Name" , name);
        QJsonArray arr;
        for(auto &e : createForm.getRows()){
            QJsonObject tmp;
            tmp.insert(e.first , QJsonValue(e.second));
            arr.push_back(tmp);
        }
        obj.insert("Description" , QJsonValue(arr));
        tot.insert("Command" , QJsonValue("createTable"));
        tot.insert("Params" , QJsonObject(obj));
        this->socket->write(QJsonDocument(tot).toJson());
        qRequests->append(QJsonDocument(tot).toJson());
    }
/*    CreateForm createForm;
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
    }*/
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
    QJsonObject tot , obj;
    obj.insert("Name" , QJsonValue(last_table));
    tot.insert("Command" , QJsonValue("addRow"));
    tot.insert("Params" , QJsonValue(obj));
    socket->write(QJsonDocument(tot).toJson());
    qRequests->append(QJsonDocument(tot).toJson());
}

void DataViewer::on_makeProjection_clicked()
{
    ShowTable(last_table);
    ProjectionTable pt(ui->dbTable);
    pt.exec();
}

void DataViewer::on_crossJoinButton_clicked()
{
    ShowTable(last_table);
//    CrossJoin crossJoin(ui->dbTable , &this->db , last_table);
//    crossJoin.exec();
//    if(crossJoin.getResult()){
//       ShowTable(crossJoin.getResultTable());
//    }
}


void DataViewer::on_dbDeleteButton_clicked()
{
    QJsonObject tot;
    tot.insert("Command" , "deleteDataBase");
    socket->write(QJsonDocument(tot).toJson());
    qRequests->append(QJsonDocument(tot).toJson());
    QDialog::reject();
}

void DataViewer::on_intersectionButton_clicked()
{
    ShowTable(last_table);
    QJsonObject tot;
    tot.insert("Command" , QJsonValue("getPossibleIntegsection"));
    tot.insert("Table" , QJsonValue(last_table));
    socket->write(QJsonDocument(tot).toJson());
    //qRequests->append(QJsonDocument(tot).toJson());
    //Intersection intersection(ui->dbTable , {} , last_table);
    //intersection.exec();
    //if(intersection.getResult()){
    //    qDebug() << " I N T E R S E C T I O N ";
    //    //   ShowTable(intersection.getResultTable());
    //}
}
