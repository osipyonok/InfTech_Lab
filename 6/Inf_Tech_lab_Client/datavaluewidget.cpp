#include "datavaluewidget.h"
#include <QDebug>

DataValueWidget::DataValueWidget(DataTypes _type, QString _val , int _i , int _j , QWidget *parent) : QWidget(parent)
{
    type = _type;
    val = _val;
    memset(range , 0, sizeof range);
    row = _i;
    col = _j;
    if(type == Int){
        auto cur = new QSpinBox(parent);
        cur->setMinimum(INT_MIN);
        cur->setMaximum(INT_MAX);
        cur->setValue(val.toInt());
        connect(cur, SIGNAL(valueChanged(int)) , this, SLOT(valueChanged(int)));
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(cur);
        this->setLayout(layout);
    }else if(type == Real){
        auto cur = new QDoubleSpinBox(parent);
        cur->setMinimum(DBL_MIN);
        cur->setMaximum(DBL_MAX);
        cur->setDecimals(6);
        cur->setValue(val.toDouble());
        connect(cur, SIGNAL(valueChanged(double)) , this, SLOT(valueChanged(double)));
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(cur);
        this->setLayout(layout);
    }else if(type == Char){
        auto cur = new QLineEdit(parent);
        cur->setText(val);
        cur->setMaxLength(1);
        connect(cur , SIGNAL(textChanged(QString)) , this , SLOT(valueChangedChar(QString)));
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(cur);
        this->setLayout(layout);
    }else if(type == String){
        auto cur = new QLineEdit(parent);
        cur->setText(val);
        connect(cur , SIGNAL(textChanged(QString)) , this , SLOT(textChanged(QString)));
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(cur);
        this->setLayout(layout);
    }else if(type == Range){
        QStringList tmp = val.split(QRegExp(R"((\[|\]|\;))"));
        tmp.removeAll("");
        if(tmp.size() == 2){
            tmp.append("1");
        }

        for(int i = 0 ; i < 3 ; ++i)
            range[i] = tmp.at(i).toInt();

        QHBoxLayout *layout = new QHBoxLayout();

        auto first_label = new QLabel(parent);
        first_label->setText("Початок:");
        layout->addWidget(first_label);


        auto first_element = new QSpinBox(parent);
        first_element->setMinimum(INT_MIN);
        first_element->setMaximum(INT_MAX);
        first_element->setValue(range[0]);
        connect(first_element , SIGNAL(valueChanged(int)) , this , SLOT(rangeFirstChanged(int)));
        layout->addWidget(first_element);


        auto last_label = new QLabel(parent);
        last_label->setText("Кiнець:");
        layout->addWidget(last_label);


        auto last_element = new QSpinBox(parent);
        last_element->setMinimum(INT_MIN);
        last_element->setMaximum(INT_MAX);
        last_element->setValue(range[1]);
        connect(last_element , SIGNAL(valueChanged(int)) , this , SLOT(rangeLastChanged(int)));
        layout->addWidget(last_element);


        auto step_label = new QLabel(parent);
        step_label->setText("Крок:");
        layout->addWidget(step_label);


        auto step = new QSpinBox(parent);
        step->setMinimum(INT_MIN);
        step->setMaximum(INT_MAX);
        step->setValue(range[2]);
        connect(step , SIGNAL(valueChanged(int)) , this , SLOT(rangeStepChanged(int)));
        layout->addWidget(step);


        this->setLayout(layout);
    }else if(type == Txtfile){
        if(val.length() > 3){
            file_name = (val.split(';').first()).split('[').last();
            file_content = val.mid(file_name.length() + 2 , val.length() - file_name.length() - 3);
        }

        temp_file_name = genFileName();
        if(file_name.length() == 0)temp_file_name += ".html";//txt

        QHBoxLayout *layout = new QHBoxLayout();

        auto fileNameLabel = new QLabel(parent);
        fileNameLabel->setText(file_name.length() ? file_name : "Файл не вибрано");
        layout->addWidget(fileNameLabel);


        auto fileOpenButton = new QPushButton(parent);
        fileOpenButton->setText("Вiдкрити");
        connect(fileOpenButton , SIGNAL(clicked()) , this , SLOT(onFileOpenButtonClicked()));

        if(file_name.length() == 0){
            fileOpenButton->setVisible(false);
        }
        layout->addWidget(fileOpenButton);

        auto fileSelectButton = new QPushButton(parent);
        fileSelectButton->setText("Вибрати");
        connect(fileSelectButton , SIGNAL(clicked()) , this , SLOT(onFileSelectButtonClicked()));
        layout->addWidget(fileSelectButton);

        auto fileDeleteButton = new QPushButton(parent);
        fileDeleteButton->setText("Видалити");
        connect(fileDeleteButton , SIGNAL(clicked()) , this , SLOT(onFileDelete()));
        layout->addWidget(fileDeleteButton);

        this->setLayout(layout);
    }
}

void DataValueWidget::valueChanged(int new_value){
    val = QString::number(new_value);
    qDebug() << val << endl;
}

void DataValueWidget::valueChanged(double new_value){
    val = QString::number(new_value);
    qDebug() << val << endl;
}

void DataValueWidget::valueChangedChar(QString str){
    if(str.length() == 0){
        val = str;
    }else{
        val = str.at(0);
    }
    qDebug() << "Val: " << val << endl;
}

void DataValueWidget::textChanged(QString str){
    val = str;
    qDebug() << "Val: " << val << endl;
}

void DataValueWidget::rangeFirstChanged(int el){
    range[0] = el;
    val = "[" + QString::number(range[0]) + ";" + QString::number(range[1]) + ";" + QString::number(range[2]) + "]";
    qDebug() << "Val: " << val << endl;
}

void DataValueWidget::rangeLastChanged(int el){
    range[1] = el;
    val = "[" + QString::number(range[0]) + ";" + QString::number(range[1]) + ";" + QString::number(range[2]) + "]";
    qDebug() << "Val: " << val << endl;
}

void DataValueWidget::rangeStepChanged(int el){
    range[2] = el;
    val = "[" + QString::number(range[0]) + ";" + QString::number(range[1]) + ";" + QString::number(range[2]) + "]";
    qDebug() << "Val: " << val << endl;
}

void DataValueWidget::onFileOpenButtonClicked(){
    QFileInfo file(temp_file_name);
    if((file.exists() && file.isFile()) == false){
        if(QDir("temp\\").exists() == false){
            QDir().mkdir("temp\\");
        }
        QFile makeFile(temp_file_name);
        makeFile.open(QIODevice::WriteOnly);
        QTextStream out(&makeFile);
        qDebug() << "The file is: " << file_content << endl;
        out.setCodec("UTF-8");
        out << file_content.toUtf8();
       // makeFile.write(file_content.toUtf8());
        makeFile.close();
    }
    QDesktopServices::openUrl(QUrl("file:" + temp_file_name));
    qDebug() << "File open clicked\n";
    val = "[" + file_name + ";" + file_content + "]";
}

void DataValueWidget::onFileSelectButtonClicked(){
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::ExistingFile);
    file_dialog.setNameFilter(tr("Text file (*.txt)"));
   // file_dialog.setNameFilter(tr("HTML file (*.html)"));
    file_dialog.exec();
    if(file_dialog.selectedFiles().size() == 0){
        make_file_open(file_name.length());
        return;
    }
    QString path = file_dialog.selectedFiles().last();
    if(path.length() == 0){
        make_file_open(file_name.length());
        return;
    }
    QFile file(path);
    QFileInfo fileinfo(path);
    if(fileinfo.exists() && fileinfo.isFile()){
        file_content = "";
        file.open(QFile::ReadOnly);
        QTextStream in(&file);
        in.setCodec("UTF-8");
        while(!in.atEnd()){
            file_content += in.readLine();
            file_content += '\n';
        }
   //     file_content = in.readAll().toUtf8();
        file_name = QFileInfo(path).fileName();
        QFile(this->temp_file_name).remove();
        file.close();
    }
    for(int i = 0 ; i < this->layout()->count() ; ++i){
        if(this->layout()->itemAt(i)->widget()->inherits("QLabel")){
            ((QLabel*)this->layout()->itemAt(i)->widget())->setText(file_name.length() ? file_name : "Файл не вибрано");
        }
    }
    make_file_open(file_name.length());
 //   ((QLabel*)this->layout()->itemAt(0))->setText(file_name.length() ? file_name : "Файл не вибрано");
//    qDebug() << this->layout()->count() << endl;
    qDebug() << "File select clicked\n";
    qDebug() << "New content " << file_content << endl;
    qDebug() << "New name " << file_name << endl;
    val = "[" + file_name + ";" + file_content + "]";
}

void DataValueWidget::onFileDelete(){
    QFile(temp_file_name).remove();
    file_name = "";
    file_content = "";
    for(int i = 0 ; i < this->layout()->count() ; ++i){
        if(this->layout()->itemAt(i)->widget()->inherits("QLabel")){
            ((QLabel*)this->layout()->itemAt(i)->widget())->setText(file_name.length() ? file_name : "Файл не вибрано");
        }
    }
    make_file_open(false);
    val = "[" + file_name + ";" + file_content + "]";
}

QString DataValueWidget::genFileName(){
    QString str = "";
    for(int i = 0 ; i < 16 ; ++i){
        bool e = rand() & 1;
        char v = rand() % 26;
        str += (e ? 'A' : 'a') + v;
    }
    return "temp\\" + str + "___" + file_name;
}

void DataValueWidget::make_file_open(bool val){
    for(int i = 0 ; i < this->layout()->count() ; ++i){
        if(this->layout()->itemAt(i)->widget()->inherits("QPushButton")){
            if(((QPushButton*)this->layout()->itemAt(i)->widget())->text() == "Вiдкрити"){
                ((QPushButton*)this->layout()->itemAt(i)->widget())->setVisible(val);
            }
        }
    }
}

QString DataValueWidget::getValue(){
    return this->val;
}

int DataValueWidget::getRow(){
    return row;
}

int DataValueWidget::getCol(){
    return col;
}
