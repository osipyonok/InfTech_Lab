#include "mainwindow.h"
#include "ui_mainwindow.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    UNIT_TEST
//    UNIT_TEST_DATATYPES
//    UNIT_TEST_CREATETABLE
//    UNIT_TEST_INTERSECTION
}

MainWindow::~MainWindow()
{
    QDir dir("temp\\");
    dir.removeRecursively();
    delete ui;
}

void MainWindow::on_Close_button_clicked()
{
    this->close();
}

void MainWindow::on_OpenDB_button_clicked()
{
    QFileDialog file;
    file.setFileMode(QFileDialog::ExistingFile);
    file.setNameFilter(tr("Inf tech lab database (*.dblab)"));
    file.exec();
    if(file.selectedFiles().size() == 0){
        return;
    }
    QString path = file.selectedFiles().last();
    QJsonDocument doc = readJson(path);
    if(!path.length() or doc.isNull() or doc.isEmpty() or !doc.object().keys().contains("name")){
        QMessageBox error;
        error.setWindowTitle("Помилка");
        error.setText("Не вдалося прочитати файл.");
        error.setIcon(QMessageBox::Critical);
        error.exec();
        return;
    }
    LoginForm login_form(doc.object());
    login_form.exec();
    if(login_form.is_authenticated()){
        DataViewer dataviewer(login_form.getObject(), path , login_form.getLogin(), login_form.getPassword());
        this->close();
        dataviewer.exec();
        this->show();
    }else{
        QMessageBox error;
        error.setWindowTitle("Помилка");
        error.setText("Невірний логін або пароль.");
        error.setIcon(QMessageBox::Critical);
        error.exec();
    }
}

void MainWindow::on_CreateDB_button_clicked()
{
    CreateDataBase f;
    f.exec();
    if(f.getResult()){
        DataBase db(f.getName() , f.getLogin() , f.getPassword());
        DataViewer dw(db.toJson().object() , "" , f.getLogin() , f.getPassword());
        this->close();
        dw.exec();
        this->show();
    }
}
