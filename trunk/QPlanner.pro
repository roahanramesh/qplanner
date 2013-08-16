#-------------------------------------------------
#
# Project created by QtCreator 2012-10-10T13:42:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QPlanner
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    plan.cpp \
    tasksmodel.cpp \
    resourcesmodel.cpp \
    calendarsmodel.cpp \
    daysmodel.cpp \
    task.cpp \
    resource.cpp \
    calendar.cpp \
    day.cpp \
    predecessors.cpp \
    tasksdelegate.cpp \
    timespanspinbox.cpp \
    ganttview.cpp \
    ganttscale.cpp \
    ganttchart.cpp \
    xdatetime.cpp \
    ganttdata.cpp \
    taskres.cpp \
    resourcesdelegate.cpp \
    xtableview.cpp

HEADERS  += mainwindow.h \
    plan.h \
    tasksmodel.h \
    resourcesmodel.h \
    calendarsmodel.h \
    daysmodel.h \
    task.h \
    resource.h \
    calendar.h \
    day.h \
    propertieswidget.h \
    commandpropertieschange.h \
    predecessors.h \
    timespan.h \
    commandtasksetdata.h \
    tasksdelegate.h \
    timespanspinbox.h \
    ganttview.h \
    ganttscale.h \
    ganttchart.h \
    xdatetime.h \
    ganttdata.h \
    commandresourcesetdata.h \
    commandtaskindent.h \
    commandtaskoutdent.h \
    taskres.h \
    resourcesdelegate.h \
    task_schedule.h \
    xtableview.h

FORMS    += mainwindow.ui
