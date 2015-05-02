#pragma once
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QString>
#include <QStringList>
#include <QObject>
#include "jsonparser.h"

class SendMessage : public QObject
{
    Q_OBJECT
public:
    SendMessage();
    QString sendMessage(const QString &token, const QString &person, const QString &message);
private slots:
    void finishedSlot(QNetworkReply *replyOnPost);
private:
    QString token;
    QNetworkAccessManager accAPI;
    QString reply;
    QEventLoop loop;
};
