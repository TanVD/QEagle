#include "vkauth.h"


VKAuth::VKAuth(const QString &appid, QWidget* parent) : QWebView(parent)
{
    connect(this, SIGNAL(urlChanged(QUrl)), SLOT(slotUrlChanged(QUrl)));
    QUrl url("https://oauth.vk.com/authorize?client_id=" + appid + "&display=popup&scope=messages,friends&redirect_uri=https://oauth.vk.com/blank.html&response_type=token");
    this->load(url);
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
    emit doneAuthenticationOfVK();
    loop.exit();
}


