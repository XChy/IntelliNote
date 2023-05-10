#ifndef NOTEMANAGER_H
#define NOTEMANAGER_H

#include <qhash.h>
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
     * default value may be
     * QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)?
     *****************************************************************************/

    void setNotesDirectory(const QString &dir);
    QString getNotesDirectory() const;

    /******************************************************************************
     * Function: readAll
     * Description:
     * read all notes, directories and tags
     * if the directory for all notes does not exist, create it
     * Return:
     * 0 : succeed
     * 1 : fail
     *****************************************************************************/
    int readAll();

    /******************************************************************************
     * Function: createNote
     * Return:
     * 0 : succeed
     * 1 : note has exist
     * 2 : other exceptions
     *****************************************************************************/

    int createNote(const Note &note);

    /******************************************************************************
     * Function: importNote
     * Return:
     * 0 : succeed
     * 1 : note has exist
     * 2 : other exceptions
     *****************************************************************************/
    int importNote(const Note &note, const QString &note_content);

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
    QList<QString> allDirs();

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
    QHash<QString, QList<Note> > dirToNotes;
    QHash<QString, QList<Note> > tagToNotes;

    QString notesDirectory;
};

#endif  // NOTEMANAGER_H
