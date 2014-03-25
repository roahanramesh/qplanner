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

SOURCES += main.cpp \
    calendar.cpp \
    calendarsmodel.cpp \
    day.cpp \
    daysmodel.cpp \
    employment.cpp \
    ganttchart.cpp \
    ganttdata.cpp \
    ganttscale.cpp \
    ganttview.cpp \
    maintabwidget.cpp \
    mainwindow.cpp \
    plan.cpp \
    predecessors.cpp \
    resource.cpp \
    resourcesdelegate.cpp \
    resourcesmodel.cpp \
    resourceworkiterator.cpp \
    task.cpp \
    taskresources.cpp \
    tasksdelegate.cpp \
    tasksmodel.cpp \
    timespanspinbox.cpp \
    xdatetime.cpp \
    xtableview.cpp

HEADERS  += \
    calendar.h \
    calendarsmodel.h \
    commandpropertieschange.h \
    commandresourcesetdata.h \
    commandtaskindent.h \
    commandtaskoutdent.h \
    commandtasksetdata.h \
    day.h \
    daysmodel.h \
    employment.h \
    ganttchart.h \
    ganttdata.h \
    ganttscale.h \
    ganttview.h \
    maintabwidget.h \
    mainwindow.h \
    plan.h \
    predecessors.h \
    propertieswidget.h \
    resource.h \
    resourcesdelegate.h \
    resourcesmodel.h \
    resourceworkiterator.h \
    task.h \
    task_schedule.h \
    taskresources.h \
    tasksdelegate.h \
    tasksmodel.h \
    timespan.h \
    timespanspinbox.h \
    xdatetime.h \
    xtableview.h

FORMS    += mainwindow.ui \
    maintabwidget.ui
