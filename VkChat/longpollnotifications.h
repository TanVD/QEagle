#pragma once
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QString>
#include <QStringList>
#include <QObject>
#include "jsonparser.h"

class LongPollNotifications : public QObject
{
    Q_OBJECT
public:
    LongPollNotifications(const QStringList &params);

public slots:
    void passQueryToServer();
    void finishedSlot(QNetworkReply *replyOnPost);
signals:
    void newMessagesAdded(QStringList);
    void somebodyIsWriting(QStringList);
    void nobodyIsWriting();
    void someMessagesWereRead();
private:
    QString key;
    QString server;
    QString ts;
    QNetworkAccessManager accAPI;
    QString reply;
    QEventLoop loop;
};
