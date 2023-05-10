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

void TransSession::translate(const QString &words) {
    //Step1. 将请求参数中的 APPID(appid)， 翻译 query(q，注意为UTF-8编码)，随机数(salt)，以及平台分配的密钥(可在管理控制台查看) 按照 appid+q+salt+密钥的顺序拼接得到字符串 1。
    //Step2. 对字符串 1 做 MD5 ，得到 32 位小写的 sign。
    //请求方式： 可使用 GET 或 POST 方式，如使用 POST 方式，Content-Type 请指定为：application/x-www-form-urlencoded
    //字符编码：统一采用 UTF-8 编码格式
    //query 长度：为保证翻译质量，请将单次请求长度控制在 6000 bytes以内（汉字约为输入参数 2000 个）
    QUrl url("https://fanyi-api.baidu.com/api/trans/vip/translate");
    QString salt = "123456";
    QString sign = QString(QCryptographicHash::hash((APP_ID + words + salt + KEY).toUtf8(),QCryptographicHash::Md5).toHex());
    QString to = "zh";
    QUrlQuery query;
    query.addQueryItem("q", words.toUtf8());
    query.addQueryItem("from", "auto");
    query.addQueryItem("to", to);
    query.addQueryItem("appid", APP_ID);
    query.addQueryItem("salt", salt);
    query.addQueryItem("sign", sign);
    url.setQuery(query);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    reply = manager->get(request);
    //connect and read from the reply
    QObject::connect(reply,&QNetworkReply::finished,[this]()
    {
        if (reply->error() == QNetworkReply::NoError) {
            //{
            //    "from": "en",
            //    "to": "zh",
            //    "trans_result": [
            //        {
            //            "src": "apple",
            //            "dst": "苹果"
            //        }
            //    ]
            //}
            QString response;
            // read the response from the json file and convert it to QString
            QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromLocal8Bit(reply->readAll()).toUtf8());
            // then get the text from the json file clear the " " and \n in both
            // sides
          response = jsonDoc.object()
                   .value("trans_result").toArray()[0]
                    .toObject().value("dst")
                  .toString();
         if(response.isEmpty())
              response = jsonDoc.object().value("error_msg").toString();
            emit completed(response);
        }else{
            QByteArray data = reply->readAll();
            QString errorString = reply->errorString();
            QNetworkReply::NetworkError error = reply->error();
            qDebug() << "error: " << errorString;
        }
    }
    );
}