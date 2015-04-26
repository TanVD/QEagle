/********************************************************************************
** Form generated from reading UI file 'chatmain.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATMAIN_H
#define UI_CHATMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatMain
{
public:
    QWidget *centralWidget;
    QTextEdit *HistoryBox;
    QTextEdit *textEdit_2;
    QPushButton *pushButton;
    QComboBox *comboBox;
    QPushButton *RefreshButton;
    QLabel *NewMessageLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ChatMain)
    {
        if (ChatMain->objectName().isEmpty())
            ChatMain->setObjectName(QStringLiteral("ChatMain"));
        ChatMain->resize(782, 596);
        centralWidget = new QWidget(ChatMain);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        HistoryBox = new QTextEdit(centralWidget);
        HistoryBox->setObjectName(QStringLiteral("HistoryBox"));
        HistoryBox->setGeometry(QRect(40, 50, 531, 371));
        HistoryBox->setReadOnly(true);
        textEdit_2 = new QTextEdit(centralWidget);
        textEdit_2->setObjectName(QStringLiteral("textEdit_2"));
        textEdit_2->setGeometry(QRect(40, 440, 531, 81));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(618, 470, 111, 41));
        comboBox = new QComboBox(centralWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(570, 50, 211, 31));
        comboBox->setMaxVisibleItems(100);
        RefreshButton = new QPushButton(centralWidget);
        RefreshButton->setObjectName(QStringLiteral("RefreshButton"));
        RefreshButton->setGeometry(QRect(718, 90, 61, 51));
        NewMessageLabel = new QLabel(centralWidget);
        NewMessageLabel->setObjectName(QStringLiteral("NewMessageLabel"));
        NewMessageLabel->setGeometry(QRect(580, 90, 121, 31));
        ChatMain->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ChatMain);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 782, 25));
        ChatMain->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ChatMain);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ChatMain->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ChatMain);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ChatMain->setStatusBar(statusBar);

        retranslateUi(ChatMain);

        comboBox->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(ChatMain);
    } // setupUi

    void retranslateUi(QMainWindow *ChatMain)
    {
        ChatMain->setWindowTitle(QApplication::translate("ChatMain", "ChatMain", 0));
        pushButton->setText(QApplication::translate("ChatMain", "Send", 0));
        RefreshButton->setText(QApplication::translate("ChatMain", "Refresh", 0));
        NewMessageLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ChatMain: public Ui_ChatMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATMAIN_H
