#include "chatmain.h"

ChatMain::ChatMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatMain)
{
    ui->setupUi(this);
    aut = new VKAuth("4892953", this);
    autServ = nullptr;
    numberOfNewMessages = 0;
    connect(aut, SIGNAL(done()), this, SLOT(onAuthDone()));
    connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(onSendButtonClicked()));
    connect(ui->contactList, SIGNAL(currentIndexChanged(int)), this, SLOT(onContactListIndexChanged(int)));
}

ChatMain::~ChatMain()
{
    notifyThread->quit();
    delete ui;
}

void ChatMain::newMessages(QStringList listNew)
{
    QMutex mutex;
    mutex.lock();
    updateFriendsList();
    for (int j = 0; j < listNew.length(); j++)
    {
        int index = -1;
        for (int i = 0; i < listOfContacts.length(); i++)
        {
            if (listOfContacts[i].first == listNew[j])
            {
                index = i;
                break;
            }
        }
        if (index != -1)
        {
            if (unreadMessages[index] == 0)
                ui->contactList->setItemText(index, "⇒ " + listOfContacts[index].second);
            unreadMessages[index]++;
            numberOfNewMessages++;
        }
    }

    ui->NewMessageLabel->setText("New message " + QString::number(numberOfNewMessages));
    //ПОЧЕМУ ЗДЕСЬ UI->contactList->CURRRENTINDEX == -1??
    mutex.unlock();
    operate();
}

void ChatMain::onAuthDone()
{
    ui->HistoryBox->verticalScrollBar()->setTracking(true);

    if (autServ != nullptr)
        delete autServ;
    autServ = new LongPollServerAuth(aut->getToken());
    serv = new LongPollNotifications(autServ->getAuth());

    notifyThread = new QThread();
    serv->moveToThread(notifyThread);
    connect(notifyThread, &QThread::finished, serv, &QObject::deleteLater);
    connect(serv, SIGNAL(getNewMessages(QStringList)), this, SLOT(newMessages(QStringList)));
    connect(serv, SIGNAL(somebodyIsWriting(QStringList)), this, SLOT(newPersonWriting(QStringList)));
    connect(serv, SIGNAL(nobodyIsWriting()), this, SLOT(nobodyWriting()));
    connect(serv, SIGNAL(someMessagesWereRead()), this, SLOT(updateHistoryBySignal()));

    connect(this, SIGNAL(operate()), serv, SLOT(passQueryToServer()));
    notifyThread->start();
    operate();

    updateFriendsList();
    ui->HistoryBox->verticalScrollBar()->setValue(ui->HistoryBox->verticalScrollBar()->maximum());
}

void ChatMain::newPersonWriting(QStringList list)
{
    updateFriendsList();
    for (int j = 0; j < list.length(); j++)
    {
        for (int i = 0; i < listOfContacts.length(); i++)
        {
            if (list[j] == listOfContacts[i].first)
            {
                writingPeople[i] = 1;
                break;
            }
        }
    }
    setLabel();
    operate();
}

void ChatMain::nobodyWriting()
{
    updateFriendsList();
    for (int i = 0; i < writingPeople.length(); i++)
    {
        writingPeople[i] = 0;
    }
    setLabel();
    operate();
}

void ChatMain::updateHistoryBySignal()
{
    updateHistoryText(ui->contactList->currentIndex());
    operate();
}

void ChatMain::updateFriendsList()
{
    QString currentId;
    if (ui->contactList->currentIndex() != -1)
         currentId = listOfContacts[ui->contactList->currentIndex()].first;

    FriendsList* list = new FriendsList();

    QList<QPair<QString, QString>> newList = list->getFriends(aut->getToken());
    if (newList.length() == 0)
        return;
    QList<QPair<QString, QString>> oldContactList = listOfContacts;
    listOfContacts = newList;

    int indexToLookPerson = 0;
    if (ui->contactList->currentIndex() != -1)
    {
        for (int i = 0; i < listOfContacts.length(); i++)
        {
            if (currentId == listOfContacts[i].first)
            {
                indexToLookPerson = i;
                break;
            }
        }
    }

    unreadMessages = syncToNewContactList(unreadMessages, oldContactList);
    writingPeople = syncToNewContactList(writingPeople, oldContactList);

    ui->contactList->blockSignals(true);
    ui->contactList->clear();
    for (int i = 0; i < listOfContacts.length(); i++)
    {
        ui->contactList->addItem(listOfContacts[i].second);
    }
    ui->contactList->blockSignals(false);
    ui->contactList->setCurrentIndex(indexToLookPerson);

    for (int i = 0; i < listOfContacts.length(); i++)
    {
        if (unreadMessages[i] > 0)
            ui->contactList->setItemText(i, "⇒ " + listOfContacts[i].second);
    }

    updateHistoryText(indexToLookPerson);
}

void ChatMain::updateHistoryText(int arg1)
{
    //HOT FIX for vertical slider fixing. Think about normal solution.
    int max = ui->HistoryBox->verticalScrollBar()->maximum();
    double valueOfScroll = ui->HistoryBox->verticalScrollBar()->sliderPosition();
    double percent = valueOfScroll / max;

    GetHistory hist;
    QStringList historyList = hist.getHistory(aut->getToken(), listOfContacts[arg1].first);
    QString historyTextBox;
    for (int i  = historyList.length() - 1; i >= 0; i--)
    {
        historyTextBox += historyList[i] + "\n";
    }
    ui->HistoryBox->setText(historyTextBox);

    ui->HistoryBox->verticalScrollBar()->setValue((int) (ui->HistoryBox->verticalScrollBar()->maximum() * percent));

    numberOfNewMessages -= unreadMessages[ui->contactList->currentIndex()];
    unreadMessages[ui->contactList->currentIndex()] = 0;
    ui->NewMessageLabel->setText("New message " + QString::number(numberOfNewMessages));
    ui->contactList->setItemText(ui->contactList->currentIndex(), listOfContacts[arg1].second);
}

QList<int> ChatMain::syncToNewContactList(QList<int> oldListSmth, QList<QPair<QString, QString>> oldContactList)
{
    QList<int> newList;
    for (int i = 0; i < listOfContacts.length(); i++)
    {
        newList.append(0);
    }
    for (int i = 0; i < oldContactList.length(); i++)
    {
        int index = listOfContacts.indexOf(oldContactList[i]);
        if (index != -1)
        {
            newList[index] = oldListSmth[i];
        }
    }
    return newList;
}

void ChatMain::setLabel()
{
    if (writingPeople[ui->contactList->currentIndex() != 0])
    {
        ui->SomebodyIsWriting->setText("Собеседник пишет вам...");
    }
    else
    {
        ui->SomebodyIsWriting->setText("");
    }
}

void ChatMain::onContactListIndexChanged(int arg1)
{
    updateHistoryText(arg1);
    ui->HistoryBox->verticalScrollBar()->setValue(ui->HistoryBox->verticalScrollBar()->maximum());
}

void ChatMain::onSendButtonClicked()
{
    QString msgToUser = ui->sendField->toPlainText();
    if (msgToUser != "" && ui->contactList->currentIndex() != -1)
    {
        SendMessage msg;
        msg.sendMessage(aut->getToken(), listOfContacts[ui->contactList->currentIndex()].first, ui->sendField->toPlainText());
        updateHistoryText(ui->contactList->currentIndex());
        ui->sendField->setText("");
    }
}

