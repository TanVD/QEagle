#pragma once
#include <QString>
#include <QStringList>

class JSONParser
{
public:
    static QStringList parseListIds(QString list);
    static QStringList parseListNames(QString list);
    static QStringList parseHistory(QStringList dialog, QString person);
    static QStringList parseLongPollAuth(QString url);
    static QStringList parseLongPollAnswer(QString answer);
};

