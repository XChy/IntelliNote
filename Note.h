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

    friend QDataStream& operator<<(QDataStream& output, const Note& note)
    {
        output << note.name;
        output << note.dir;
        output << note.path;
        output << (note.type == ExternalNote ? 'E' : 'I');
        return output;
    }

    friend QDataStream& operator>>(QDataStream& input, Note& note)
    {
        input >> note.name;
        input >> note.dir;
        input >> note.path;
        QChar type_identity;
        input >> type_identity;
        if (type_identity == 'E')
            note.type = ExternalNote;
        else
            note.type = InnerNote;
        return input;
    }

   public:
    Note();

    // one note has only one directory as its parent
    QString dir;

    QString name;

    QString path;

    NoteType type;
};

#endif  // NOTE_H
