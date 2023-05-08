#ifndef GPTSESSION_H
#define GPTSESSION_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

static const char *default_api_key =
    "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";
static const int maxTokens = 100;

class GPTSession : public QObject
{
    Q_OBJECT
   public:
    explicit GPTSession(QObject *parent = nullptr);

    /******************************************************************************
     * Function:         void addPrompt(const QString& prompt)
     * Description:
     * add prompt to GPT, and emit the signal 'responseReceived(QString)'
     * when GPT respond
     *****************************************************************************/
    void ask(const QString &prompt);
    bool askable() const;

    void stopAsk();

    void setAPIKey(const QString &apiKey);
    QString getAPIKey() const;

   signals:
    /******************************************************************************
     * Function:         void responseReceived(QString response)
     * Description:
     * emit this signal when GPT respond
     *****************************************************************************/
    void responseReceived(QString response);

   private:
    QStringList responses;
    QString APIKey;

    QNetworkAccessManager *manager;
    QNetworkReply *reply;
};

#endif  // GPTSESSION_H
