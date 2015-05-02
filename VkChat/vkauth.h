#pragma once
#include <QWebView>
#include <QUrl>
#include <QString>
#include <QObject>
#include <QEventLoop>

class VKAuth : public QWebView
{
    Q_OBJECT
public:
    VKAuth(const QString &appid, QWidget* parent);
    QString getToken();
    int getExpiration();
private slots:
    void slotUrlChanged(QUrl url);
signals:
    void doneAuthenticationOfVK();
private:
    QString token;
    int expires;
    QEventLoop loop;
};

