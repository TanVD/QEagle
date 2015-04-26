#pragma once
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QCoreApplication>

class LongPollNotifications : public QObject
{
    Q_OBJECT
public:
    LongPollNotifications(QStringList params);

public slots:
    void passQueryToServer();
    void finishedSlot(QNetworkReply *replyOnPost);
signals:
    void getNewMessages(QStringList);
private:
    QString key;
    QString server;
    QString ts;
    QNetworkAccessManager accAPI;
    QString reply;
    QEventLoop loop;
};
