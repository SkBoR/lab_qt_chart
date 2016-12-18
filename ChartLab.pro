#-------------------------------------------------
#
# Project created by QtCreator 2016-12-09T21:24:17
#
#-------------------------------------------------

QT       += core gui charts testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChartLab
TEMPLATE = app

SUBDIRS += ChartLabTest

SOURCES += main.cpp\
        mainwindow.cpp \
    parser.cpp \
    transientanalysisentity.cpp \
    parseexception.cpp \
    detaildialog.cpp \
    colormanager.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    parser.h \
    transientanalysisentity.h \
    parseexception.h \
    detaildialog.h \
    colormanager.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    detaildialog.ui \
    aboutdialog.ui

RESOURCES += \
    resources.qrc
