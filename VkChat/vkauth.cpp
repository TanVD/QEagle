#include "vkauth.h"
#include <QWebView>
#include <QUrlQuery>
#include <QUrl>

VKAuth::VKAuth(QString appid, QWidget* parent) : QWebView(parent)
{
    connect(this, SIGNAL(urlChanged(QUrl)), SLOT(slotUrlChanged(QUrl)));
    QUrlQuery url("https://oauth.vk.com/authorize?");
    url.addQueryItem("client_id", appid);
    url.addQueryItem("display", "popup");
    url.addQueryItem("scope", "messages,friends");
    url.addQueryItem("redirect_uri", "https://oauth.vk.com/blank.html ");
    url.addQueryItem("response_type", "token");
    this->load(QUrl(url.toString()));
    this->adjustSize();
}

QString VKAuth::getToken()
{
    return token;
}

int VKAuth::getExpiration()
{
    return expires;
}

void VKAuth::slotUrlChanged(QUrl url)
{
    QString urlString = url.fragment();
    if (urlString.contains("error"))
        return;
    if (!urlString.contains("access_token"))
        return;
    for (int i = urlString.indexOf("=") + 1; i < urlString.indexOf("&"); i++)
        token += urlString[i];
    QString expiresString;
    for (int i = urlString.indexOf("expires_in=") + 1; i < urlString.indexOf("&"); i++)
        expiresString += urlString[i];
    expires = expiresString.toInt();
    this->hide();
    emit done();
    loop.exit();
}


