#include "chatmain.h"

ChatMain::ChatMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatMain)
{
    ui->setupUi(this);
    vkAuthentication = new VKAuth("4892953", this);
    longPollAuthentication = nullptr;
    numberOfNewMessages = 0;
    connect(vkAuthentication, SIGNAL(doneAuthenticationOfVK()), this, SLOT(onAuthVKDone()));
    connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(onSendButtonClicked()));
    connect(ui->contactList, SIGNAL(currentIndexChanged(int)), this, SLOT(onContactListIndexChanged(int)));
}

ChatMain::~ChatMain()
{
    longPollServerThread->quit();
    delete ui;
}

void ChatMain::onNewMessageAdded(const QStringList &listNew)
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
            if (unreadMessagesList[index] == 0)
                ui->contactList->setItemText(index, "⇒ " + listOfContacts[index].second);
            unreadMessagesList[index]++;
            numberOfNewMessages++;
        }
    }

    ui->NewMessageLabel->setText("New message " + QString::number(numberOfNewMessages));
    //ПОЧЕМУ ЗДЕСЬ UI->contactList->CURRRENTINDEX == -1??
    mutex.unlock();
    startLongPoll();
}

void ChatMain::onAuthVKDone()
{
    ui->HistoryBox->verticalScrollBar()->setTracking(true);

    if (longPollAuthentication != nullptr)
        delete longPollAuthentication;
    longPollAuthentication = new LongPollServerAuth(vkAuthentication->getToken());
    longPollServer = new LongPollNotifications(longPollAuthentication->getAuthentication());

    longPollServerThread = new QThread();
    longPollServer->moveToThread(longPollServerThread);
    connect(longPollServerThread, &QThread::finished, longPollServer, &QObject::deleteLater);
    connect(longPollServer, SIGNAL(newMessagesAdded(QStringList)), this, SLOT(onNewMessageAdded(QStringList)));
    connect(longPollServer, SIGNAL(somebodyIsWriting(QStringList)), this, SLOT(onNewPersonWriting(QStringList)));
    connect(longPollServer, SIGNAL(nobodyIsWriting()), this, SLOT(onNobodyWriting()));
    connect(longPollServer, SIGNAL(someMessagesWereRead()), this, SLOT(onHistoryUpdated()));

    connect(this, SIGNAL(startLongPoll()), longPollServer, SLOT(passQueryToServer()));
    longPollServerThread->start();
    startLongPoll();

    updateFriendsList();
    ui->HistoryBox->verticalScrollBar()->setValue(ui->HistoryBox->verticalScrollBar()->maximum());
}

void ChatMain::onNewPersonWriting(const QStringList &list)
{
    updateFriendsList();
    for (int j = 0; j < list.length(); j++)
    {
        for (int i = 0; i < listOfContacts.length(); i++)
        {
            if (list[j] == listOfContacts[i].first)
            {
                writingPeopleList[i] = 1;
                break;
            }
        }
    }
    updateLabelBuddyWriting();
    startLongPoll();
}

void ChatMain::onNobodyWriting()
{
    updateFriendsList();
    for (int i = 0; i < writingPeopleList.length(); i++)
    {
        writingPeopleList[i] = 0;
    }
    updateLabelBuddyWriting();
    startLongPoll();
}

void ChatMain::onHistoryUpdated()
{
    updateHistoryText(ui->contactList->currentIndex());
    startLongPoll();
}

void ChatMain::updateFriendsList()
{
    QString currentId;
    if (ui->contactList->currentIndex() != -1)
         currentId = listOfContacts[ui->contactList->currentIndex()].first;

    FriendsList* list = new FriendsList();

    QList<QPair<QString, QString>> newList = list->getFriends(vkAuthentication->getToken());
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

    unreadMessagesList = syncToNewContactList(unreadMessagesList, oldContactList);
    writingPeopleList = syncToNewContactList(writingPeopleList, oldContactList);

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
        if (unreadMessagesList[i] > 0)
            ui->contactList->setItemText(i, "⇒ " + listOfContacts[i].second);
    }

    updateHistoryText(indexToLookPerson);
}

//Медленно
void ChatMain::updateHistoryText(int arg1)
{
    //HOT FIX for vertical slider fixing. Think about normal solution.
    int max = ui->HistoryBox->verticalScrollBar()->maximum();
    double valueOfScroll = ui->HistoryBox->verticalScrollBar()->sliderPosition();
    double percent = valueOfScroll / max;

    GetHistory hist;
    QStringList historyList = hist.getHistory(vkAuthentication->getToken(), listOfContacts[arg1].first);
    QString historyTextBox;
    for (int i  = historyList.length() - 1; i >= 0; i--)
    {
        historyTextBox += historyList[i] + "\n";
    }
    ui->HistoryBox->setText(historyTextBox);

    ui->HistoryBox->verticalScrollBar()->setValue((int) (ui->HistoryBox->verticalScrollBar()->maximum() * percent));

    numberOfNewMessages -= unreadMessagesList[ui->contactList->currentIndex()];
    unreadMessagesList[ui->contactList->currentIndex()] = 0;
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

void ChatMain::updateLabelBuddyWriting()
{
    if (writingPeopleList[ui->contactList->currentIndex() != 0])
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
        msg.sendMessage(vkAuthentication->getToken(), listOfContacts[ui->contactList->currentIndex()].first, ui->sendField->toPlainText());
        updateHistoryText(ui->contactList->currentIndex());
        ui->sendField->setText("");
    }
}

