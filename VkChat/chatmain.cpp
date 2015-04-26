#include "chatmain.h"
#include "ui_chatmain.h"
#include "vkauth.h"
#include "friendslist.h"
#include <QPair>
#include <QList>
#include <QThread>
#include "gethistory.h"
#include "longpollserverauth.h"
#include "sendmessage.h"
#include "longpollnotifications.h"

ChatMain::ChatMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatMain)
{
    ui->setupUi(this);
    aut = new VKAuth("4892953", this);
    autServ = nullptr;
    numberOfNewMessages = 0;
    connect(ui->RefreshButton, SIGNAL(clicked()), this, SLOT(onRefreshButtonClicked()));
}

ChatMain::~ChatMain()
{
    notifyThread->quit();
    delete ui;
}

void ChatMain::onRefreshButtonClicked()
{
    if (autServ != nullptr)
        delete autServ;
    autServ = new LongPollServerAuth(aut->getToken());  
    serv = new LongPollNotifications(autServ->getAuth());
    notifyThread = new QThread();
    serv->moveToThread(notifyThread);
    connect(notifyThread, &QThread::finished, serv, &QObject::deleteLater);
    connect(serv, SIGNAL(getNewMessages(QStringList)), this, SLOT(newMessages(QStringList)));
    connect(this, SIGNAL(operate()), serv, SLOT(passQueryToServer()));
    notifyThread->start();
    operate();
    FriendsList* list = new FriendsList();
    ui->comboBox->blockSignals(true);
    ui->comboBox->clear();
    listOfContacts = list->getFriends(aut->getToken());
    for (int i = 0; i < listOfContacts.length(); i++)
    {
        ui->comboBox->addItem(listOfContacts[i].second);
    }
    ui->comboBox->blockSignals(false);
}


void ChatMain::newMessages(QStringList listNew)
{
    FriendsList* list = new FriendsList();
    ui->comboBox->blockSignals(true);
    ui->comboBox->clear();
    listOfContacts = list->getFriends(aut->getToken());
    for (int i = 0; i < listOfContacts.length(); i++)
    {
        ui->comboBox->addItem(listOfContacts[i].second);
    }
    ui->comboBox->blockSignals(false);

    GetHistory hist;
    QStringList historyList = hist.getHistory(aut->getToken(), listOfContacts[ui->comboBox->currentIndex()].first);
    QString historyTextBox;
    for (int i  = historyList.length() - 1; i >= 0; i--)
    {
        historyTextBox += historyList[i] + "\n";
    }
    ui->HistoryBox->setText(historyTextBox);

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
        if (index != -1 && ui->comboBox->itemText(index)[0] != '⇒')
            ui->comboBox->setItemText(index, "⇒ " + listOfContacts[index].second);
    }
    numberOfNewMessages += listNew.length();
    ui->NewMessageLabel->setText("New message " + QString::number(numberOfNewMessages));
    operate();
}

void ChatMain::on_comboBox_currentIndexChanged(int arg1)
{
    GetHistory hist;
    QStringList historyList = hist.getHistory(aut->getToken(), listOfContacts[arg1].first);
    QString historyTextBox;
    for (int i  = historyList.length() - 1; i >= 0; i--)
    {
        historyTextBox += historyList[i] + "\n";
    }
    ui->HistoryBox->setText(historyTextBox);
    ui->comboBox->setItemText(arg1, listOfContacts[arg1].second);
    if (ui->comboBox->itemText(arg1)[0] == '⇒')
    {
        numberOfNewMessages--;//А ЕСЛИ ДВА? Использовать list
        ui->NewMessageLabel->setText("New message " + QString::number(numberOfNewMessages));
    }

}

void ChatMain::on_pushButton_clicked()
{
    QString msgToUser = ui->textEdit_2->toPlainText();
    if (msgToUser != "")
    {
        SendMessage msg;
        msg.sendMessage(aut->getToken(), listOfContacts[ui->comboBox->currentIndex()].first, ui->textEdit_2->toPlainText());
        GetHistory hist;
        QStringList historyList = hist.getHistory(aut->getToken(), listOfContacts[ui->comboBox->currentIndex()].first);
        QString historyTextBox;
        for (int i  = historyList.length() - 1; i >= 0; i--)
        {
            historyTextBox += historyList[i] + "\n";
        }
        ui->HistoryBox->setText(historyTextBox);
        ui->textEdit_2->setText("");
    }
}

