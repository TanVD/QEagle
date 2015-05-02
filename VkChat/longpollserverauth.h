#pragma once
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QString>
#include <QStringList>
#include <QObject>
#include "jsonparser.h"

class LongPollServerAuth : public QObject
{
    Q_OBJECT
public:
    LongPollServerAuth(const QString &token);
    QStringList getAuthentication();
private slots:
    void finishedSlot(QNetworkReply *replyOnPost);
private:
    QString token;
    QStringList authList;
    QNetworkAccessManager accAPI;
    QString reply;
    QEventLoop loop;
};
