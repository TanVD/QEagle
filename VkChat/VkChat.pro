#-------------------------------------------------
#
# Project created by QtCreator 2015-04-25T20:11:23
#
#-------------------------------------------------

QT       += core gui

QT       += xml webkit network

QT       += webkitwidgets



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VkChat
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        chatmain.cpp \
    vkauth.cpp \
    friendslist.cpp \
    jsonparser.cpp \
    gethistory.cpp \
    longpollserverauth.cpp \
    sendmessage.cpp \
    longpollnotifications.cpp

HEADERS  += chatmain.h \
    vkauth.h \
    friendslist.h \
    jsonparser.h \
    gethistory.h \
    longpollserverauth.h \
    sendmessage.h \
    longpollnotifications.h

FORMS    += chatmain.ui
