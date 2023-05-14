#include "NoteManager.h"
#include <qdir.h>
#include <qlist.h>
#include "Note.h"

NoteManager::NoteManager(QObject *parent) : QObject{parent} {}

void NoteManager::setNotesDirectory(const QString &dir)
{
    notesDirectory = dir;
}

QString NoteManager::getNotesDirectory() const { return notesDirectory; }

int NoteManager::readAll()
{
    QDir dir(notesDirectory);

    if (!dir.exists()) {
        dir.mkdir(notesDirectory);
        dir.mkdir("Notes");
    }

    // TODO: read note
    QDir notes_dir(notesDirectory + "/Notes");
    for (auto dir_name : notes_dir.entryList(QDir::Dirs)) {
        QDir subdir(notes_dir.path() + "/" + dir_name);
        dirToNotes[dir_name] = QList<Note>();

        for (auto note_name : subdir.entryList(QDir::Files)) {
            Note note;
            note.name = note_name;
            note.path = subdir.absolutePath() + "/" + note_name;
            note.dir = dir_name;
            note.type = Note::InnerNote;

            dirToNotes[dir_name].append(note);
            notes.append(note);
        }
    }

    readTags();
    return 0;
}

int NoteManager::createNote(const Note &note)
{
    // check whether the note duplicate
    for (auto compared_note : notes) {
        if (note.name == compared_note.name && note.dir == compared_note.dir) {
            return 1;
        }
    }

    QString note_path =
        QString("%1/%2/%3").arg(notesDirectory, note.dir, note.name);
    dirToNotes[note.dir].append(note);
    notes.append(note);

    // create file for note
    QFile note_file(note_path);
    if (!note_file.open(QFile::ReadWrite)) {
        return 2;
    }
    note_file.close();

    return 0;
}

int NoteManager::importNote(const Note &note, const QString &external_note_path)
{
    // check whether the note duplicate
    for (auto compared_note : notes) {
        if (note.name == compared_note.name && note.dir == compared_note.dir) {
            return 1;
        }
    }

    QString note_path =
        QString("%1/%2/%3").arg(notesDirectory, note.dir, note.name);
    dirToNotes[note.dir].append(note);
    notes.append(note);

    // import file for note

    if (!QFile::copy(external_note_path, note_path)) return 2;

    return 0;
}

int NoteManager::removeNote(const Note &note)
{
    // remove from all notes
    bool found = false;

    for (int i = 0; i < notes.size(); ++i) {
        if (note.name == notes[i].name && note.dir == notes[i].dir) {
            notes.remove(i);
            found = true;
            break;
        }
    }

    if (!found) return 1;

    // remove files
    if (!QFile::remove(note.path)) return 2;

    // remove from directories
    auto notelist = dirToNotes[note.dir];
    for (int i = 0; i < notelist.size(); ++i) {
        if (note.name == notelist[i].name && note.dir == notelist[i].dir) {
            notelist.remove(i);
            break;
        }
    }

    // remove from tags
    for (auto &notelist : tagToNotes) {
        for (int i = 0; i < notelist.size(); ++i) {
            if (note.name == notelist[i].name && note.dir == notelist[i].dir) {
                notelist.remove(i);
                break;
            }
        }
    }
    saveTags();

    return 0;
}

int NoteManager::removeDir(const QString &dir)
{
    if (!dirToNotes.contains(dir)) return 1;

    dirToNotes.remove(dir);

    for (int i = 0; i < notes.size(); ++i) {
        if (notes[i].dir == dir) notes.remove(i);
    }

    QDir(notesDirectory + "/" + dir).removeRecursively();

    return 0;
}

QString NoteManager::readNote(const Note &note) const
{
    QFile file(note.path);
    file.open(QFile::ReadOnly);
    QString result = file.readAll();
    file.close();
    return result;
}

int NoteManager::saveNote(const Note &note, const QString &content)
{
    QFile file(note.path);
    if (!file.open(QFile::WriteOnly)) return 1;

    file.write(content.toUtf8());
    file.close();
    return 0;
}

int NoteManager::readTags()
{
    QFile file(notesDirectory + "/tags");
    if (!file.open(QFile::ReadOnly)) return 1;
    QDataStream stream(&file);
    stream >> tagToNotes;
    file.close();

    return 0;
}

int NoteManager::saveTags()
{
    QFile file(notesDirectory + "/tags");
    if (!file.open(QFile::WriteOnly)) return 1;
    QDataStream stream(&file);
    stream << tagToNotes;
    file.close();

    return 0;
}

QList<Note> NoteManager::allNotes() const { return notes; }

QList<QString> NoteManager::allDirs() const { return dirToNotes.keys(); }

QStringList NoteManager::allTags() const { return tagToNotes.keys(); }

QList<Note> NoteManager::notesOfDir(const QString &dir) const
{
    return dirToNotes[dir];
}

QList<Note> NoteManager::notesOfTag(const QString &tag) const
{
    return tagToNotes[tag];
}
