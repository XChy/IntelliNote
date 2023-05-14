#include <QApplication>
#include <QStandardPaths>
#include <QDebug>
#include "NoteManager.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    NoteManager* manager = new NoteManager(&app);
    manager->setNotesDirectory(
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
        "/IntelliNote");
    manager->readAll();

    // check readAll
    for (auto note : manager->allNotes()) {
        qDebug() << note.dir << "/" << note.name;
    }

    return 0;
}
