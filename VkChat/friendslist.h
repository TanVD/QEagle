#pragma once
#include <QUrl>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QNetworkReply>
#include "jsonparser.h"

class FriendsList : public QObject
{
    Q_OBJECT
public:
    FriendsList();
    QList<QPair<QString, QString>> getFriends(const QString &token);
private slots:
    void finishedSlot(QNetworkReply *replyOnPost);
private:
    QStringList getNames(const QStringList &friendsId);
    QString token;
    QNetworkAccessManager accAPI;
    QString reply;
    QEventLoop loop;
};

