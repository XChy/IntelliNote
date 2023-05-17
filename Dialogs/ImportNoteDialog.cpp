#include "ImportNoteDialog.h"
#include <qfileinfo.h>
#include <qlist.h>
#include <qpushbutton.h>
#include <QFileDialog>
#include "Note.h"
#include "NoteManager.h"
#include "ui_ImportNoteDialog.h"

ImportNoteDialog::ImportNoteDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ImportNoteDialog)
{
    ui->setupUi(this);
}
ImportNoteDialog::ImportNoteDialog(QWidget *parent, NoteManager *manager)
    : QDialog(parent), ui(new Ui::ImportNoteDialog), manager(manager)
{
    ui->setupUi(this);
    connect(manager, &NoteManager::noteChanged, [this]() {
        ui->comboBox->clear();
        ui->comboBox->addItems(this->manager->allDirs());
    });

    connect(ui->pushButton, &QPushButton::pressed, [this]() {
        QStringList paths = QFileDialog::getOpenFileNames(
            this, tr("Select the notes to be imported"));
        ui->lineEdit->setText(paths.join(";"));
    });
}

QStringList ImportNoteDialog::getPaths() const
{
    return ui->lineEdit->text().split(";");
}

QList<Note> ImportNoteDialog::getNotes() const
{
    auto paths = getPaths();
    QList<Note> notes;
    for (auto path : paths) {
        Note note;
        note.name = QFileInfo(path).baseName();
        note.dir = ui->comboBox->currentText();
        note.path = manager->pathForInternal(note);
        note.type = Note::InnerNote;
        notes.append(note);
    }
    return notes;
}

ImportNoteDialog::~ImportNoteDialog() { delete ui; }
