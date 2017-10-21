#-------------------------------------------------
#
# Project created by QtCreator 2017-10-12T23:24:58
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Inf_Tech_lab_Client
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
    databasesocket.cpp \
    createdatabase.cpp \
    opendb.cpp \
    dataviewer.cpp \
    datarow.cpp \
    datatable.cpp \
    datavaluewidget.cpp \
    createform.cpp \
    projectiontable.cpp

HEADERS += \
        mainwindow.h \
    databasesocket.h \
    createdatabase.h \
    opendb.h \
    dataviewer.h \
    datarow.h \
    datavaluewidget.h \
    datatypes.h \
    datatable.h \
    additional_functions.h \
    createform.h \
    projectiontable.h

FORMS += \
        mainwindow.ui \
    createdatabase.ui \
    opendb.ui \
    dataviewer.ui \
    createform.ui \
    projectiontable.ui
