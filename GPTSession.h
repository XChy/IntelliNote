#ifndef GPTSESSION_H
#define GPTSESSION_H

#include <QObject>

static const char *default_api_key = "";

class GPTSession : public QObject
{
    Q_OBJECT
   public:
    explicit GPTSession(QObject *parent = nullptr);

    /******************************************************************************
     * Function:         void addPrompt(const QString& prompt)
     * Description:
     * add prompt to GPT, and emit the signal 'responseReceived(QString)'
     * when GPT respond Return: Error:
     *****************************************************************************/
    void addPrompt(const QString &prompt);

    void setAPIKey(const QString &apiKey);
    QString getAPIKey();

   signals:
    void reponseReceived(QString response);
   private:
    QStringList responses;
    QString APIKey;
};

#endif  // GPTSESSION_H
