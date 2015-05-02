#include "friendslist.h"

FriendsList::FriendsList()
{
     connect(&accAPI, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
}

QList<QPair<QString, QString>> FriendsList::getFriends(const QString &token)
{
    this->token = token;
    QString url("https://api.vk.com/method/friends.getOnline?order=hints&access_token=" + token);
    accAPI.get(QNetworkRequest(url));
    loop.exec();

    QStringList ids = JSONParser::parseListIds(reply);
    QStringList names = getNames(ids);

    QList<QPair<QString, QString>> result;
    //На случай ошибки сервера
    if (names.length() == 0 || ids.length() == 0 || (names.length() != ids.length()))
        return result;
    //Гарантированно ли возвращение в том же порядке?
    for (int i = 0; i < ids.length(); i++)
    {
        QPair<QString, QString> pair(ids[i], names[i]);
        result.append(pair);
    }
    return result;
}

QStringList FriendsList::getNames(const QStringList &friendsId)
{
    QString url("https://api.vk.com/method/users.get?user_ids=");
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
