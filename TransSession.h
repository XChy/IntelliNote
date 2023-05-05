#ifndef TRANSSESSION_H
#define TRANSSESSION_H

#include <QObject>

class TransSession : public QObject
{
    Q_OBJECT
   public:
    explicit TransSession(QObject *parent = nullptr);

    void translate(const QString &words);

   signals:
    void completed(QString translated);
};

#endif  // TRANSSESSION_H
