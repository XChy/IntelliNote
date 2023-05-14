#include <QApplication>
#include <QStandardPaths>
#include <QDebug>
#include <QFile>
#include "NoteManager.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    NoteManager* manager = new NoteManager(&app);
    manager->setNotesDirectory(
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
        "/IntelliNote");
    manager->readAll();

    auto notesDirectory = manager->getNotesDirectory();
    QFile file(notesDirectory + "/tags");
    if (!file.open(QFile::ReadOnly)) return 1;
    QDataStream stream(&file);
    while (!stream.atEnd()) {
        QString s;
        stream >> s;
        qDebug() << s;
    }
    file.close();

    // check readAll
    for (auto note : manager->allNotes()) {
        qDebug() << note.dir << "/" << note.name;
    }

    // check tags
    for (auto tag : manager->allTags()) {
        qDebug() << tag;
    }

    // check createDir
    for (int i = 0; i < 10; ++i) {
        int err = manager->createDir(QString::number(i));
    }

    // check createNote
    for (int i = 0; i < 10; ++i) {
        Note note;
        note.dir = QString::number(i);
        note.name = QString::number(i);
        note.path = manager->pathForInternal(note);
        int err = manager->createNote(note);
    }

    // check saveNote
    for (int i = 0; i < 10; ++i) {
        Note note;
        note.dir = QString::number(i);
        note.name = QString::number(i);
        note.path = manager->pathForInternal(note);
        int err = manager->saveNote(note, QString::number(i));
    }

    // check readNote
    for (int i = 0; i < 10; ++i) {
        Note note;
        note.dir = QString::number(i);
        note.name = QString::number(i);
        note.path = manager->pathForInternal(note);
        qDebug() << manager->readNote(note);
    }
    // check tagNote
    for (int i = 0; i < 10; ++i) {
        Note note;
        note.dir = QString::number(i);
        note.name = QString::number(i);
        note.path = manager->pathForInternal(note);
        manager->tagNote(note, note.name);
    }

    return 0;
}
