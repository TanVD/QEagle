#include "friendslist.h"
#include <QUrlQuery>
#include <QNetworkReply>
#include "jsonparser.h"

FriendsList::FriendsList()
{
     connect(&accAPI, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
}

QList<QPair<QString, QString>> FriendsList::getFriends(QString token)
{
    this->token = token;
    QString url("https://api.vk.com/method/friends.getOnline?");
    url += "order=hints";
    url += "&access_token=" + token;
    accAPI.get(QNetworkRequest(url));
    loop.exec();
    QStringList ids = JSONParser::parseListIds(reply);
    QStringList names = nameFriends(ids);
    QList<QPair<QString, QString>> result;
    //Гарантированно ли возвращение в том же порядке?
    for (int i = 0; i < ids.length(); i++)
    {
        QPair<QString, QString> pair(ids[i], names[i]);
        result.append(pair);
    }
    return result;
}

QStringList FriendsList::nameFriends(QStringList friendsId)
{
    QString url("https://api.vk.com/method/users.get?");
    url += "user_ids=";
    for (int i = 0; i < friendsId.length(); i++)
    {
        url += friendsId[i] + ",";
    }
    url.chop(1);
    accAPI.get(QNetworkRequest(url));
    loop.exec();
    QStringList newList = JSONParser::parseListNames(reply);
    return newList;
}

void FriendsList::finishedSlot(QNetworkReply *replyOnPost)
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
