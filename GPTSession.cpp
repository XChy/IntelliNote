#include "GPTSession.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

GPTSession::GPTSession(QObject *parent)
    : QObject{parent}, APIKey(default_api_key)
{
}

void GPTSession::setAPIKey(const QString &apiKey) { APIKey = apiKey; }

QString GPTSession::getAPIKey() { return APIKey; }

void GPTSession::addPrompt(const QString &prompt)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("https://api.openai.com/v1/completions");
    //    QUrlQuery params;
    //    params.addQueryItem("prompt", prompt);
    //    params.addQueryItem("max_tokens", QString::number(maxTokens));
    //    url.setQuery(params);
    QNetworkRequest request(url);
    // the QNetworkRequest class holds a request to be sent with
    // QNetworkAccessManager.
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + APIKey).toUtf8());
    // token means APIKey
    QNetworkReply *reply =
        manager->post(request, QString("{\n"
                                       "\"model\" : \"gpt-3.5-turbo\","
                                       " \"prompt\": \"%1\" "
                                       "\n}")
                                   .arg(prompt)
                                   .toUtf8());
    QObject::connect(reply, &QNetworkReply::finished, [reply, manager, this]() {
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
                           .value("text")
                           .toString()
                           .remove(0, 2);
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
        manager->deleteLater();
    });
}
