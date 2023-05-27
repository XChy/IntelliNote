#include "TransSession.h"
#include <QUrlQuery>
#include <QCoreApplication>

#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

TransSession::TransSession(QObject *parent)
    : QObject{parent},
      APP_ID(default_APP_ID),
      KEY(default_KEY),
      manager(new QNetworkAccessManager(this)),
      reply(nullptr)
{
}
void TransSession::translate(const QString &words)
{
   //签名生成方法如下：
    //signType=v3；
    //sign=sha256(应用ID+input+salt+curtime+应用密钥)；
    //其中，input的计算方式为：input=q前10个字符 + q长度 + q后10个字符（当q长度大于20）或 input=q字符串（当q长度小于等于20）；
    QUrl url("https://openapi.youdao.com/api");
    QString salt = "123456";

    //get current time by utc and change it into seconds as a string
    ushort curtime=QDateTime::currentDateTimeUtc().toSecsSinceEpoch();
    QString sign =QString(QCryptographicHash::hash((APP_ID + words + salt + QString(curtime) + KEY).toUtf8(),
                                         QCryptographicHash::Sha256)
                    .toHex());
    QString to = "zh-CHS";
    QString signtype = "v3";
    QUrlQuery query;
    query.addQueryItem("q", words.toUtf8());
    query.addQueryItem("from", "auto");
    query.addQueryItem("to", to);
    query.addQueryItem("appKey", default_APP_ID);
    query.addQueryItem("salt", salt);
    query.addQueryItem("sign", sign);
    query.addQueryItem("signType", signtype);
    query.addQueryItem("curtime", QString(curtime));
    url.setQuery(query);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");
    reply = manager->get(request);
    // connect and read from the reply
    QObject::connect(reply, &QNetworkReply::finished, [this]() {
        if (reply->error() == QNetworkReply::NoError) {
            //{
            //  "errorCode":"0",
            //  "query":"good", //查询正确时，一定存在
            //  "isDomainSupport":"true", //翻译结果是否为领域翻译(仅开通领域翻译时存在)
            //  "translation": [ //查询正确时一定存在
            //      "好"
            //  ],
            //  "basic":{ // 有道词典-基本词典,查词时才有
            //      "phonetic":"gʊd",
            //      "uk-phonetic":"gʊd", //英式音标
            //      "us-phonetic":"ɡʊd", //美式音标
            //      "uk-speech": "XXXX",//英式发音
            //      "us-speech": "XXXX",//美式发音
            //      "explains":[
            //          "好处",
            //          "好的",
            //          "好",
            //      ]
            //  },
            //  "web":[ // 有道词典-网络释义，该结果不一定存在
            //      {
            //          "key":"good",
            //          "value":["良好","善","美好"]
            //      },
            //      {...}
            //  ],
            //  "dict":{
            //      "url":"yddict://m.youdao.com/dict?le=eng&q=good"
            //  },
            //  "webdict":{
            //      "url":"http://m.youdao.com/dict?le=eng&q=good"
            //  },
            //  "l":"EN2zh-CHS",
            //  "tSpeakUrl":"XXX",//翻译后的发音地址
            //  "speakUrl": "XXX" //查询文本的发音地址
            //}
            QString response = reply->readAll();
            // read the response from the json file and convert it to QString
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response.toUtf8());
            // then get the text from the json file clear the " " and \n in both
            // sides
            /*response = jsonDoc.object()
                           .value("translation")
                            .toString();*/
            if (response.isEmpty())
                response = jsonDoc.object().value("error_msg").toString();
            emit completed(response);
        } else {
            QByteArray data = reply->readAll();
            QString errorString = reply->errorString();
            QNetworkReply::NetworkError error = reply->error();
            qDebug() << "error: " << errorString;
        }
    });
}
