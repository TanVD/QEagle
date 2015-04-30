#pragma once
#include <QMainWindow>
#include <QPair>
#include <QList>
#include <QThread>
#include <QScrollBar>
#include <QMutex>
#include "vkauth.h"
#include "longpollserverauth.h"
#include "longpollnotifications.h"
#include "ui_chatmain.h"
#include "vkauth.h"
#include "friendslist.h"
#include "gethistory.h"
#include "longpollserverauth.h"
#include "sendmessage.h"
#include "longpollnotifications.h"


namespace Ui {
class ChatMain;
}

class ChatMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatMain(QWidget *parent = 0);
    ~ChatMain();

signals:
    void operate();

private slots:
    void onContactListIndexChanged(int arg1);

    void onSendButtonClicked();

    void newMessages(QStringList listNew);

    void onAuthDone();

    void newPersonWriting(QStringList list);

    void nobodyWriting();

    void updateHistoryBySignal();

private:
    void updateFriendsList();
    void updateHistoryText(int arg1);
    QList<int> syncToNewContactList(QList<int> oldListSmth, QList<QPair<QString, QString> > oldContactList);

    QList<QPair<QString, QString>> listOfContacts;

    LongPollNotifications* serv;
    int numberOfNewMessages;
    QList<int> unreadMessages;

    QList<int> writingPeople;
    void setLabel();

    Ui::ChatMain *ui;

    VKAuth* aut;
    LongPollServerAuth* autServ;

    QThread* notifyThread;
};

