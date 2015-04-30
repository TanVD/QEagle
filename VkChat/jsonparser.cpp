#include "jsonparser.h"

QStringList JSONParser::parseListIds(QString list)
{
    QStringList newList;
    while(list.indexOf("[") != -1)
    {
        if (list.indexOf(",") != -1)
        {
            newList.append(list.mid(list.indexOf("[") + 1, list.indexOf(",") - list.indexOf("[") - 1));
            list.remove(list.indexOf("["), list.indexOf(",") - list.indexOf("["));
            list[list.indexOf(",")] = '[';
        }
        else
        {
            newList.append(list.mid(list.indexOf("[") + 1, list.indexOf("]") - list.indexOf("[") - 1));
            list.remove(list.indexOf("["), list.indexOf("]") - list.indexOf("["));
        }
    }
    return newList;
}

QStringList JSONParser::parseListNames(QString list)
{
    QStringList newList;
    int i = 0;
    while(list.indexOf("first_name\":\"") != -1 || list.indexOf("last_name\":\"") != -1)
    {
        if (i % 2 == 0)
        {
            int pos = list.indexOf("first_name\":\"");
            newList.append(list.mid(pos + 13, list.indexOf("\"", pos + 13) - pos - 13));
            list.remove(pos, list.indexOf("\"", pos + 13) - pos - 12);
        }
        else
        {
            int pos = list.indexOf("last_name\":\"");
            newList[i / 2] += " " + list.mid(pos + 12, list.indexOf("\"", pos + 12) - pos - 12);
            list.remove(pos, list.indexOf("\"", pos + 12) - pos - 11);
        }
        i++;
    }
    return newList;
}

QStringList JSONParser::parseHistory(QStringList dialog, QString person)
{
    QStringList newList;
    for (int i = 0; i < dialog.length(); i++)
    {
        QString sentence = dialog[i];
        QString answer = sentence.mid(1, sentence.indexOf("\"", 1) - 1);
        if (!sentence.contains("from_id"))
            continue;
        bool from = false;
        QString fromWhom = sentence.mid(sentence.indexOf("from_id\":") + 9, sentence.indexOf(",", sentence.indexOf("from_id\":") + 9) - sentence.indexOf("from_id\":") - 9);
        if ((fromWhom) == person)
        {
            from = true;
        }
        bool read = false;
        QString readState = sentence.mid(sentence.indexOf("read_state\":") + 12, sentence.indexOf(",", sentence.indexOf("read_state\":") + 12) - sentence.indexOf("read_state\":") - 12);
        if (readState == "1")
        {
            read = true;
        }
        if (from)
        {
            answer = "Собеседник: " + answer;
        }
        else
        {
            answer = "Я: " + answer;
        }
        if (!read)
        {
            answer = "!- " + answer;
        }
        newList.append(answer);
    }
    return newList;
}

QStringList JSONParser::parseLongPollAuth(QString url)
{
    QStringList newList;
    int pos = url.indexOf("key\":\"");
    newList.append(url.mid(pos + 6, url.indexOf("\"", pos + 6) - pos - 6));
    url.remove(pos, url.indexOf("\"", pos + 6) - pos - 5);
    pos = url.indexOf("server\":\"");
    newList.append(url.mid(pos + 9, url.indexOf("\"", pos + 9) - pos - 9));
    url.remove(pos, url.indexOf("\"", pos + 9) - pos - 8);
    pos = url.indexOf("ts\":");
    newList.append(url.mid(pos + 4, url.indexOf("}", pos + 4) - pos - 4));
    url.remove(pos, url.indexOf("\"", pos + 4) - pos - 3);
    return newList;
}

QStringList JSONParser::parseLongPollAnswerMsg(QString answer)
{
    QStringList ansList;
    while (answer.contains("[4,"))
    {
        int posBegin = answer.indexOf("[4,");
        int posEnd = answer.indexOf("]", answer.indexOf("[4,"));
        QStringList newList = answer.mid(posBegin, posEnd - posBegin).split(",");
        QString msg = newList[3];
        int flag = newList[2].toInt();
        if (flag % 4 == 3 || flag % 4 == 2)
        {
            answer.remove(posBegin - 1, posEnd - posBegin + 1);
            continue;
        }
        ansList.append(msg);
        answer.remove(posBegin - 1, posEnd - posBegin + 1);
    }
    return ansList;
}

QStringList JSONParser::parseLongPollAnswerWriting(QString answer)
{
    QStringList ansList;
    while (answer.contains("[61,"))
    {
        int posBegin = answer.indexOf("[61,");
        int posEnd = answer.indexOf("]", answer.indexOf("[61,"));
        QStringList newList = answer.mid(posBegin, posEnd - posBegin).split(",");
        QString msg = newList[1];
        ansList.append(msg);
        answer.remove(posBegin - 1, posEnd - posBegin + 1);
    }
    return ansList;
}
