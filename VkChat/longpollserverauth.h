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

class LongPollServerAuth : public QObject
{
    Q_OBJECT
public:
    LongPollServerAuth(QString token);
    QStringList getAuth();
private slots:
    void finishedSlot(QNetworkReply *replyOnPost);
private:
    QString token;
    QStringList authList;
    QNetworkAccessManager accAPI;
    QString reply;
    QEventLoop loop;
};
