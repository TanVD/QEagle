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
#include "sendmessage.h"


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
    void startLongPoll();

private slots:
    void onContactListIndexChanged(int arg1);

    void onSendButtonClicked();

    void onNewMessageAdded(const QStringList &listNew);

    void onAuthVKDone();

    void onNewPersonWriting(const QStringList &list);

    void onNobodyWriting();

    void onHistoryUpdated();

private:
    void updateFriendsList();

    void updateHistoryText(int arg1);

    void updateLabelBuddyWriting();

    QList<int> syncToNewContactList(QList<int> oldListSmth, QList<QPair<QString, QString> > oldContactList);

    QList<QPair<QString, QString>> listOfContacts;

    LongPollNotifications* longPollServer;

    int numberOfNewMessages;

    QList<int> unreadMessagesList;

    QList<int> writingPeopleList;

    Ui::ChatMain *ui;

    VKAuth* vkAuthentication;

    LongPollServerAuth* longPollAuthentication;

    QThread* longPollServerThread;
};

