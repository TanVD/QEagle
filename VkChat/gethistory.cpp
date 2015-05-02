#include "gethistory.h"


GetHistory::GetHistory()
{
     connect(&accAPI, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
}

QStringList GetHistory::getHistory(const QString &token, const QString &person)
{
    this->token = token;
    QString url("https://api.vk.com/method/messages.getHistory?count=150&user_id=" + person + "&access_token=" + token);
    accAPI.get(QNetworkRequest(url));
    loop.exec();

    QStringList newList = reply.split("body\":");
    newList = JSONParser::parseHistory(newList, person);
    return newList;
}


void GetHistory::finishedSlot(QNetworkReply *replyOnPost)
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
