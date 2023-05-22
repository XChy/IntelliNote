#include "NewNoteBookDialog.h"
#include "ui_NewNoteBookDialog.h"

NewNoteBookDialog::NewNoteBookDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::NewNoteBookDialog)
{
    ui->setupUi(this);
}

NewNoteBookDialog::~NewNoteBookDialog() { delete ui; }

QString NewNoteBookDialog::notebook() const { return ui->lineEdit->text(); }
