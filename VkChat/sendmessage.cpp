#include "sendmessage.h"

SendMessage::SendMessage()
{
    connect(&accAPI, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
}

QString SendMessage::sendMessage(const QString &token, const QString &person, const QString &message)
{
    this->token = token;
    QString url("https://api.vk.com/method/messages.send?user_id=" + person + "&message=" + message + "&access_token=" + token);
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
