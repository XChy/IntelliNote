#include "NoteManager.h"
#include <qdir.h>

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
    }
}
