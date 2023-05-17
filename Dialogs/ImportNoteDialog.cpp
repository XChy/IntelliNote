#include "ImportNoteDialog.h"
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
}

QStringList ImportNoteDialog::getPaths() const
{
    return ui->lineEdit->text().split(";");
}

QList<Note> ImportNoteDialog::getNotes() const {}

ImportNoteDialog::~ImportNoteDialog() { delete ui; }
