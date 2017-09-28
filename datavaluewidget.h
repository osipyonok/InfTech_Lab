#ifndef DATAVALUEWIDGET_H
#define DATAVALUEWIDGET_H

#include <QWidget>
#include <datatypes.h>
#include <bits/stdc++.h>
#include <QString>
#include <QHBoxLayout>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QTextStream>

using namespace std;

class DataValueWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataValueWidget(DataTypes _type, QString _val , int _i , int _j , QWidget *parent = nullptr);
    QString getValue();
    int getRow();
    int getCol();

signals:

public slots:
    void valueChanged(int);
    void valueChanged(double);
    void valueChangedChar(QString);
    void textChanged(QString);
    void rangeFirstChanged(int);
    void rangeLastChanged(int);
    void rangeStepChanged(int);
    void onFileOpenButtonClicked();
    void onFileSelectButtonClicked();
    void onFileDelete();

private:
    DataTypes type;
    QString val;
    int range[3];
    QString file_name = "";
    QString file_content = "";
    QString temp_file_name = "";
    QString genFileName();
    void make_file_open(bool);
    int row , col;
};

#endif // DATAVALUEWIDGET_H
