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

class GetHistory : public QObject
{
    Q_OBJECT
public:
    GetHistory();
    QStringList getHistory(QString token, QString person);
private slots:
    void finishedSlot(QNetworkReply *replyOnPost);
private:
    QString token;
    QNetworkAccessManager accAPI;
    QString reply;
    QEventLoop loop;
};
