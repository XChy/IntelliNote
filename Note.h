#ifndef NOTE_H
#define NOTE_H

#include <QString>
#include <QStringList>

struct Note {
   public:
    Note();

    QString parent_dir;

    QString path;

    QString name;

    QStringList tags;
};

#endif  // NOTE_H
