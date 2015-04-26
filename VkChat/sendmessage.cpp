#include "sendmessage.h"
#include <QUrlQuery>
#include <QNetworkReply>
#include "jsonparser.h"

SendMessage::SendMessage()
{
    connect(&accAPI, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
}

QString SendMessage::sendMessage(QString token, QString person, QString message)
{
    this->token = token;
    QString url("https://api.vk.com/method/messages.send?");
    url += "user_id=" + person;
    url += "&message=" + message;
    url += "&access_token=" + token;
    accAPI.get(QNetworkRequest(url));
    loop.exec();
    return reply;
}

void SendMessage::finishedSlot(QNetworkReply *replyOnPost)
{
    if (replyOnPost->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = replyOnPost->readAll();
        QString string(bytes);
        reply = string;
    }
    else
    {
        reply = "";
    }
    delete replyOnPost;
    loop.exit();
}
