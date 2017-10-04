#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <bits/stdc++.h>
#include "QDebug"
#include "database.h"
#include <datatypes.h>
#include <additional_functions.h>
#include <QRegExp>
#include <loginform.h>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <dataviewer.h>
#include <QWidget>
#include <createdatabase.h>
#include <test_datatypes.h>
#include <test_create_table.h>
#include <test_intersection.h>


#define UNIT_TEST_DATATYPES Test_datatypes test1;
#define UNIT_TEST_CREATETABLE Test_create_table test2;
#define UNIT_TEST_INTERSECTION Test_intersection test3;
#define UNIT_TEST {UNIT_TEST_DATATYPES UNIT_TEST_CREATETABLE UNIT_TEST_INTERSECTION}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Close_button_clicked();

    void on_OpenDB_button_clicked();

    void on_CreateDB_button_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
