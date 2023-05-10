#include "NoteManager.h"

NoteManager::NoteManager(QObject *parent) : QObject{parent} {}

void NoteManager::setNotesDirectory(const QString &dir)
{
    notesDirectory = dir;
}

QString NoteManager::getNotesDirectory() const { return notesDirectory; }

int NoteManager::createNote(const Note &note) {}
