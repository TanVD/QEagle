#include "longpollnotifications.h"
#include <QUrlQuery>
#include <QNetworkReply>
#include "jsonparser.h"

LongPollNotifications::LongPollNotifications(QStringList params) : key(params[0]), server(params[1]), ts(params[2])
{
    connect(&accAPI, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
}

void LongPollNotifications::passQueryToServer()
{

    int y = server.indexOf("\\");
    if (y != -1)
        server.remove(y, 1);
    QString url("https://"+ server + "?act=a_check&key=" + key + "&ts=" + ts + "&wait=5&mode=2");
    accAPI.get(QNetworkRequest(url));
}


void LongPollNotifications::finishedSlot(QNetworkReply *replyOnPost)
{
    if (replyOnPost->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = replyOnPost->readAll();
        QString string(bytes);
        reply = string;
        QString newTs = reply.mid(reply.indexOf(":") + 1, reply.indexOf(",") - reply.indexOf(":") - 1);
        ts = newTs;
        if (reply.contains("[4,"))
        {
            QStringList newList = JSONParser::parseLongPollAnswer(reply);
            emit getNewMessages(newList);
            return;
        }
    }
    else
    {
        reply = "";
    }
    delete replyOnPost;
    passQueryToServer();
}

