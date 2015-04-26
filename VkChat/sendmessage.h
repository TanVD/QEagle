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

class SendMessage : public QObject
{
    Q_OBJECT
public:
    SendMessage();
    QString sendMessage(QString token, QString person, QString message);
private slots:
    void finishedSlot(QNetworkReply *replyOnPost);
private:
    QString token;
    QNetworkAccessManager accAPI;
    QString reply;
    QEventLoop loop;
};
