#pragma once
#include <QMainWindow>
#include "vkauth.h"
#include "longpollserverauth.h"
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
    void onRefreshButtonClicked();

    void on_comboBox_currentIndexChanged(int arg1);

    void on_pushButton_clicked();

    void newMessages(QStringList listNew);

private:
    int numberOfNewMessages;
    Ui::ChatMain *ui;
    VKAuth* aut;
    LongPollServerAuth* autServ;
    QThread* notifyThread;
    QList<QPair<QString, QString>> listOfContacts;
    LongPollNotifications* serv;
};

