#include "GPTSession.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

GPTSession::GPTSession(QObject *parent)
    : QObject{parent}, APIKey(default_api_key)
{
}

void GPTSession::setAPIKey(const QString &apiKey) { APIKey = apiKey; }

QString GPTSession::getAPIKey() { return APIKey; }

void GPTSession::addPrompt(const QString &prompt)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QUrl url("https://api.openai.com/v1/engines/davinci-codex/completions");
    QUrlQuery params;
    params.addQueryItem("prompt", prompt);
    params.addQueryItem("max_tokens", QString::number(maxTokens));
    url.setQuery(params);
    QNetworkRequest request(url);
    // the QNetworkRequest class holds a request to be sent with
    // QNetworkAccessManager.
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + APIKey).toUtf8());
    // token means APIKey
    QNetworkReply *reply = manager->post(request, QByteArray());
    QObject::connect(reply, &QNetworkReply::finished, [reply, manager, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString response = QString::fromUtf8(reply->readAll());
            emit responseReceived(response);
        } else {
            // TODO: error handling
        }
        reply->deleteLater();
        manager->deleteLater();
    });
}
