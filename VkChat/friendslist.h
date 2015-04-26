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

class FriendsList : public QObject
{
    Q_OBJECT
public:
    FriendsList();
    QList<QPair<QString, QString> > getFriends(QString token);
    QStringList nameFriends(QStringList friendsId);
private slots:
    void finishedSlot(QNetworkReply *replyOnPost);
private:
    QString token;
    QNetworkAccessManager accAPI;
    QString reply;
    QEventLoop loop;
};

