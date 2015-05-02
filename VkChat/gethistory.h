#pragma once
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QString>
#include <QStringList>
#include <QObject>
#include "jsonparser.h"

class GetHistory : public QObject
{
    Q_OBJECT
public:
    GetHistory();
    QStringList getHistory(const QString &token, const QString &person);
private slots:
    void finishedSlot(QNetworkReply *replyOnPost);
private:
    QString token;
    QNetworkAccessManager accAPI;
    QString reply;
    QEventLoop loop;
};
