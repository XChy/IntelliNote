#ifndef GPTSESSION_H
#define GPTSESSION_H

#include <QObject>

static const char *default_api_key = "";
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
    void addPrompt(const QString &prompt);

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
};

#endif  // GPTSESSION_H
