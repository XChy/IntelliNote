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
     * Function: createDir
     * Return:
     * 0 : succeed
     * 1 : note has exist
     * 2 : other exceptions
     *****************************************************************************/

    int createDir(const QString &dir);

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
     * 1 : note has existed
     * 2 : other exceptions
     *****************************************************************************/
    int importNote(const Note &note, const QString &external_note_path);

    /******************************************************************************
     * Function: removeNote
     * Return:
     * 0 : succeed
     * 1 : no such note
     * 2 : other exceptions
     *****************************************************************************/
    int removeNote(const Note &note);

    /******************************************************************************
     * Function: removeDir
     * Return:
     * 0 : succeed
     * 1 : no such dir
     * 2 : other exceptions
     *****************************************************************************/
    int removeDir(const QString &dir);

    /******************************************************************************
     * Function: removeTag
     * Return:
     * 0 : succeed
     * 1 : no such dir
     * 2 : other exceptions
     *****************************************************************************/
    int removeTag(const QString &dir);

    /******************************************************************************
     * Function: renameNote
     * Return:
     * 0 : succeed
     * 1 : fail
     *****************************************************************************/
    int renameNote(const Note &note, QString new_name);

    /******************************************************************************
     * Function: renameNote
     * Return:
     * 0 : succeed
     * 1 : fail
     *****************************************************************************/
    int renameDir(const QString &old_dir, const QString &new_dir);

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
    int saveNote(const Note &note, const QString &newContent);

    /******************************************************************************
     * Function: readTags
     * Return:
     * 0 : succeed
     * 1 : fail
     *****************************************************************************/
    int readTags();

    /******************************************************************************
     * Function: saveTags
     * Return:
     * 0 : succeed
     * 1 : fail
     *****************************************************************************/
    int saveTags();

    /******************************************************************************
     * Function: tagsFor
     * Return: all tags of a note
     *****************************************************************************/
    QStringList tagsFor(const Note &note) const;

    /******************************************************************************
     * Function: tagNote
     * Return:
     * 0 : succeed
     * 1 : fail
     *****************************************************************************/
    int tagNote(const Note &note, const QString &tag);

    /******************************************************************************
     * Function: tagNote
     * Return:
     * 0 : succeed
     * 1 : fail
     *****************************************************************************/
    int untagNote(const Note &note, const QString &tag);

    /******************************************************************************
     * Function: getNotes
     * Return: if not cached, read from local files
     * Error:
     *****************************************************************************/
    QList<Note> allNotes() const;

    /******************************************************************************
     * Function: allDirs
     * Return: all directories
     *****************************************************************************/
    QList<QString> allDirs() const;

    /******************************************************************************
     * Function: allTags
     * Return: all tags managed by IntelliNote
     *****************************************************************************/
    QStringList allTags() const;

    /******************************************************************************
     * Function: notesOfDir
     * Return: get all notes of a dir
     *****************************************************************************/
    QList<Note> notesOfDir(const QString &dir) const;

    /******************************************************************************
     * Function: notesOfTag
     * Return: get all notes with such tag
     *****************************************************************************/
    QList<Note> notesOfTag(const QString &tag) const;

    QString pathForInternal(const Note &note) const;

   signals:
    void noteChanged();

   private:
    QList<Note> notes;
    QHash<QString, QList<Note> > dirToNotes;
    QHash<QString, QList<Note> > tagToNotes;

    QString notesDirectory;
};

#endif
