#ifndef NOTE_H
#define NOTE_H

#include <QString>
#include <QStringList>

struct Note {
   public:
    enum NoteType {
        // External note is the notes not imported yet, which don't belong to
        // any directory or tag
        ExternalNote,

        // Inner note is the imported notes with directory and tags
        InnerNote
    };

   public:
    Note();

    // one note has only one directory as its parent
    QString dir;

    QString path;

    QString name;

    // one note can have multiple tags
    QStringList tags;
};

#endif  // NOTE_H
