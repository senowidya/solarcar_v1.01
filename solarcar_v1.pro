#-------------------------------------------------
#
# Project created by QtCreator 2014-11-20T00:56:57
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = solarcar_v1
TEMPLATE = app


SOURCES += main.cpp\
        interfacing.cpp \
        qcustomplot/qcustomplot.cpp

HEADERS  += interfacing.h \
         qcustomplot/qcustomplot.h

FORMS    += interfacing.ui
