#include "longpollserverauth.h"

LongPollServerAuth::LongPollServerAuth(const QString &token)
{
     connect(&accAPI, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
     QString url("https://api.vk.com/method/messages.getLongPollServer?access_token=" + token);

     accAPI.get(QNetworkRequest(url));
     loop.exec();

     authList = JSONParser::parseLongPollAuth(reply);
}

void LongPollServerAuth::finishedSlot(QNetworkReply *replyOnPost)
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

QStringList LongPollServerAuth::getAuthentication()
{
    return authList;
}
