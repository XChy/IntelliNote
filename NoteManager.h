#ifndef NOTEMANAGER_H
#define NOTEMANAGER_H

#include <qlist.h>
#include <QObject>
#include "Note.h"

class NoteManager : public QObject
{
    Q_OBJECT
   public:
    explicit NoteManager(QObject *parent = nullptr);

    /******************************************************************************
     * NoteDirectory is the default directory where notes lies in
     *****************************************************************************/

    void setNoteDirectory(const QString &dir);
    QString getNoteDirectory();

    /******************************************************************************
     * Function: createNote
     * Return:
     * 0 : succeed
     * 1 : note has exist
     * 2 : other exceptions
     *****************************************************************************/

    int createNote(const Note &note);

    /******************************************************************************
     * Function: removeNote
     * Return:
     * 0 : succeed
     * 1 : no such note
     * 2 : other exceptions
     *****************************************************************************/
    int removeNote(const Note &note);

    /******************************************************************************
     * Function: readNote
     * Return:
     * the content of the note
     *****************************************************************************/
    QString readNote(const Note &note) const;

    /******************************************************************************
     * Function: saveNote
     * Description:
     * If oldNote is empty then create new note;
     * else edit the old note with the tags, content ... of newNote
     * Return:
     * 0 : succeed
     * 1 : failure
     *****************************************************************************/
    int saveNote(const Note &oldNote, const Note &newNote,
                 const QString &newContent);

    /******************************************************************************
     * Function: getNotes
     * Return: if not cached, read from local files, else return what you cache.
     * Error:
     *****************************************************************************/
    QList<Note> allNotes();

    /******************************************************************************
     * Function: allDirs
     * Return: all directories (read local if needed)
     *****************************************************************************/
    QList<Note> allDirs();

    /******************************************************************************
     * Function: allDirs
     * Return: all directories (read local if needed)
     *****************************************************************************/
    QStringList allTags();

    /******************************************************************************
     * Function: notesOfDir
     * Return: get all notes of a dir
     *****************************************************************************/
    QList<Note> notesOfDir(const QString &dir);

    /******************************************************************************
     * Function: notesOfTag
     * Return: get all notes with such tag
     *****************************************************************************/
    QList<Note> notesOfTag(const QString &tag);

   signals:

   private:
    bool hasCached;

    QList<Note> cachedNotes;
    QString noteDirectory;
};

#endif  // NOTEMANAGER_H
