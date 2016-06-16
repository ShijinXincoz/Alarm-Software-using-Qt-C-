#-------------------------------------------------
#
# Project created by QtCreator 2016-05-23T09:42:21
#
#-------------------------------------------------

QT       += core sql multimedia
QT		 -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Alarm
TEMPLATE = app


SOURCES += main.cpp\
        alarm.cpp \
    debug.cpp \
    landingpage.cpp \
    editalarm.cpp

HEADERS  += alarm.h \
    debug.h \
    main.h \
    landingpage.h \
    editalarm.h
