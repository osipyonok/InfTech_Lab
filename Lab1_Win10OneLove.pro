#-------------------------------------------------
#
# Project created by QtCreator 2017-09-25T22:56:21
#
#-------------------------------------------------

QT       += core gui
QT       += widgets
QT       += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lab1_Win10OneLove
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    database.cpp \
    datarow.cpp \
    datatable.cpp \
    datavaluewidget.cpp \
    dataviewer.cpp \
    createform.cpp \
    loginform.cpp \
    messagestack.cpp \
    createdatabase.cpp \
    projectiontable.cpp \
    intersection.cpp \
    test_datatypes.cpp \
    test_create_table.cpp \
    test_intersection.cpp

HEADERS += \
        mainwindow.h \
    additional_functions.h \
    database.h \
    datarow.h \
    datatable.h \
    datatypes.h \
    datavaluewidget.h \
    dataviewer.h \
    createform.h \
    loginform.h \
    messagestack.h \
    createdatabase.h \
    projectiontable.h \
    intersection.h \
    test_datatypes.h \
    test_create_table.h \
    test_intersection.h

FORMS += \
        mainwindow.ui \
    dataviewer.ui \
    createform.ui \
    loginform.ui \
    createdatabase.ui \
    projectiontable.ui \
    intersection.ui

STATECHARTS +=
