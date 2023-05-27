#include "TagDialog.h"
#include <qlist.h>
#include <vector>
#include "NoteManager.h"
#include "ui_TagDialog.h"

TagDialog::TagDialog(QWidget *parent) : QDialog(parent), ui(new Ui::TagDialog)
{
    ui->setupUi(this);
}
TagDialog::TagDialog(QWidget *parent, NoteManager *manager, Note note)
    : QDialog(parent), ui(new Ui::TagDialog), manager(manager), note(note)
{
    ui->setupUi(this);
}

void TagDialog::setOriginalTags(const QList<QString> &tags)
{
    std::vector<QString> completion;
    for (auto s : tags) completion.push_back(s);
    ui->tagEdit->completion(completion);
}

QList<QString> TagDialog::tags() const
{
    auto tags_vector = ui->tagEdit->tags();
    QList<QString> result;
    for (auto s : tags_vector) result.append(s);
    return result;
}

QList<QString> TagDialog::tagsToAdd() const
{
    auto originalTags = tags();
    auto tags_vector = ui->tagEdit->tags();
    QList<QString> result;
    for (auto s : tags_vector) result.append(s);
    return result;
}

QList<QString> TagDialog::tagsToRemove() const
{
    auto tags_vector = ui->tagEdit->tags();
    QList<QString> result;
    for (auto s : tags_vector) result.append(s);
    return result;
}

TagDialog::~TagDialog() { delete ui; }
