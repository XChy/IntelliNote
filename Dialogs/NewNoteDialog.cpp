#include "NewNoteDialog.h"
#include <qlist.h>
#include <vector>
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
        std::vector<QString> completion;
        for (auto s : this->manager->allTags()) completion.push_back(s);
        ui->tagEdit->completion(completion);
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

QList<QString> NewNoteDialog::tags() const
{
    QList<QString> tags;
    for (auto s : ui->tagEdit->tags()) tags.push_back(s);
    return tags;
}

void NewNoteDialog::setCurrentDir(const QString &notebook)
{
    ui->comboBox->setCurrentText(notebook);
}

NewNoteDialog::~NewNoteDialog() { delete ui; }
