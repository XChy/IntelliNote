#include "GPTSession.h"
#include <qnetworkaccessmanager.h>
#include <QCoreApplication>

#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

GPTSession::GPTSession(QObject *parent)
    : QObject{parent},
      APIKey(default_api_key),
      manager(new QNetworkAccessManager(this)),
      reply(nullptr)
{
}

void GPTSession::setAPIKey(const QString &apiKey) { APIKey = apiKey; }

QString GPTSession::getAPIKey() const { return APIKey; }

void GPTSession::ask(const QString &prompt)
{
    QUrl url("https://api.openai.com/v1/chat/completions");
    QNetworkRequest request(url);
    // the QNetworkRequest class holds a request to be sent with
    // QNetworkAccessManager.
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + APIKey).toUtf8());
    // token means APIKey
    reply = manager->post(request, QString(R"({
                                       "model" : "gpt-3.5-turbo",
                                       "messages": [{"role": "user", "content": "%1"}]
                                       })")
                                       .arg(prompt)
                                       .toUtf8());

    QObject::connect(reply, &QNetworkReply::finished, [this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString response = QString::fromUtf8(reply->readAll());
            // read the response from the json file and convert it to QString
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response.toUtf8());
            // then get the text from the json file clear the " " and \n in both
            // sides
            response = jsonDoc.object()
                           .value("choices")
                           .toArray()[0]
                           .toObject()
                           .value("message")
                           //.toArray()[0]
                           .toObject()
                           .value("content")
                           .toString();
            emit responseReceived(response);
        } else {
            // Handle error
            QByteArray data = reply->readAll();
            QString errorString = reply->errorString();
            QNetworkReply::NetworkError error = reply->error();
            qWarning() << "Network error occurred:" << errorString;
            qWarning() << "Error response:" << data;
            switch (error) {
                case QNetworkReply::ConnectionRefusedError:
                    // TODO: add error handling logic
                    break;
                case QNetworkReply::HostNotFoundError:
                    // TODO: add error handling logic
                    break;
                case QNetworkReply::TimeoutError:
                    // TODO: add error handling logic
                    break;
                default:
                    // TODO: add error handling logic
                    break;
            }
            emit responseReceived("Error");
        }
        reply->deleteLater();
        reply = nullptr;
    });
}

bool GPTSession::askable() const
{
    return reply == nullptr || !reply->isRunning();
}

void GPTSession::stopAsk()
{
    if (reply) {
        reply->abort();
        reply->deleteLater();
    }
}
