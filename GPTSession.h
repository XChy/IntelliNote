#ifndef GPTSESSION_H
#define GPTSESSION_H

#include <QObject>

class GPTSession : public QObject
{
    Q_OBJECT
public:
    explicit GPTSession(QObject *parent = nullptr);

signals:

};


#endif // GPTSESSION_H
