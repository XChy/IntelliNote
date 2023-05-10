#ifndef TRANSSESSION_H
#define TRANSSESSION_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

static const char *default_APP_ID ="20230510001673359";
static const char *default_KEY = "EbwY38JrInMB096odE4v";
class TransSession : public QObject
{
    Q_OBJECT
   public:
    explicit TransSession(QObject *parent = nullptr);

    void translate(const QString &words);

   signals:
    void completed(QString translated);
private:
    QStringList responses;
    QString APP_ID;
    QString KEY;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
};

#endif  // TRANSSESSION_H
