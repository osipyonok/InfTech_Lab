#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <test_create_table.h>
#include <test_datatypes.h>
#include <test_intersection.h>

#define UNIT_TEST_CREATE_TABLE Test_create_table t1;
#define UNIT_TEST_DATATYPES Test_datatypes t2;
#define UNIT_TEST_INTERSECTION Test_intersection t3;

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
