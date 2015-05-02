#pragma once
#include <QString>
#include <QStringList>

class JSONParser
{
public:
    static QStringList parseListIds(const QString &list);
    static QStringList parseListNames(const QString &list);
    static QStringList parseHistory(const QStringList &dialog, const QString &person);
    static QStringList parseLongPollAuth(const QString &url);
    static QStringList parseLongPollAnswerMsg(const QString &answer);
    static QStringList parseLongPollAnswerWriting(const QString &answer);
};

