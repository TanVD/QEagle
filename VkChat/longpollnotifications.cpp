#include "longpollnotifications.h"

LongPollNotifications::LongPollNotifications(const QStringList &params) : key(params[0]), server(params[1]), ts(params[2])
{
    connect(&accAPI, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
}

void LongPollNotifications::passQueryToServer()
{
    while (server.indexOf("\\") != -1)
        server.remove(server.indexOf("\\"), 1);
    QString url("https://"+ server + "?act=a_check&key=" + key + "&ts=" + ts + "&wait=5&mode=2");
    accAPI.get(QNetworkRequest(url));
}

//ПЛОДЯТСЯ ПОТОКИ
void LongPollNotifications::finishedSlot(QNetworkReply *replyOnPost)
{
    if (replyOnPost->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = replyOnPost->readAll();
        QString string(bytes);
        reply = string;
        ts = reply.mid(reply.indexOf(":") + 1, reply.indexOf(",") - reply.indexOf(":") - 1);
        if (reply.contains("[4,"))
        {
            QStringList newList = JSONParser::parseLongPollAnswerMsg(reply);
            if (newList.length() != 0)
                emit newMessagesAdded(newList);
        }
        if (reply.contains("[61,"))
        {
            QStringList newList = JSONParser::parseLongPollAnswerWriting(reply);
            emit somebodyIsWriting(newList);
        }
        else
        {
            emit nobodyIsWriting();
        }
        if (reply.contains("[7,"))
            emit someMessagesWereRead();
        //Добавить сигнал о закончившейся регистрации в лонг полл
        delete replyOnPost;
        return;
    }
    else
    {
        reply = "";
    }
    delete replyOnPost;
    passQueryToServer();
}

