#-------------------------------------------------
#
# Project created by QtCreator 2012-10-10T13:42:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QPlanner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    plan.cpp \
    tasksmodel.cpp \
    resourcesmodel.cpp \
    calendarsmodel.cpp \
    daysmodel.cpp \
    task.cpp \
    resource.cpp

HEADERS  += mainwindow.h \
    plan.h \
    tasksmodel.h \
    resourcesmodel.h \
    calendarsmodel.h \
    daysmodel.h \
    task.h \
    resource.h

FORMS    += mainwindow.ui
