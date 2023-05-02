#ifndef NOTEMANAGER_H
#define NOTEMANAGER_H

#include <QObject>

class NoteManager : public QObject
{
    Q_OBJECT
public:
    explicit NoteManager(QObject *parent = nullptr);

signals:

};

#endif // NOTEMANAGER_H
