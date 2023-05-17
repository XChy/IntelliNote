#include "NewNoteDialog.h"
#include "NoteManager.h"
#include "ui_NewNoteDialog.h"

NewNoteDialog::NewNoteDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::NewNoteDialog)
{
    ui->setupUi(this);
}
NewNoteDialog::NewNoteDialog(QWidget *parent, NoteManager *manager)
    : QDialog(parent), ui(new Ui::NewNoteDialog), manager(manager)
{
    ui->setupUi(this);
    connect(manager, &NoteManager::noteChanged, [this]() {
        ui->comboBox->clear();
        ui->comboBox->addItems(this->manager->allDirs());
    });
}

Note NewNoteDialog::getNote() const
{
    Note note;
    note.name = ui->lineEdit->text();
    note.dir = ui->comboBox->currentText();
    note.path = manager->pathForInternal(note);
    note.type = Note::InnerNote;
    return note;
}

NewNoteDialog::~NewNoteDialog() { delete ui; }
