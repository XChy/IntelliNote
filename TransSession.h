#ifndef TRANSSESSION_H
#define TRANSSESSION_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

static const char *default_APP_ID ="0ce040985da12aff";
static const char *default_KEY = "Gn7by7I5Rabj5CIAQjhLBnOX6dADstYl";
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
