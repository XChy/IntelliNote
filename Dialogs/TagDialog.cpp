#include "TagDialog.h"
#include <qlist.h>
#include <vector>
#include <QSet>
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

    auto tags = manager->tagsFor(note);
    std::vector<QString> currentTags(tags.begin(), tags.end());
    ui->tagEdit->tags(currentTags);

    setCompletion(manager->allTags());
}

void TagDialog::setCompletion(const QList<QString> &tags)
{
    std::vector<QString> completion;
    for (auto s : tags) completion.push_back(s);
    ui->tagEdit->completion(completion);
}

QStringList TagDialog::tags() const
{
    auto tags_vector = ui->tagEdit->tags();
    QList<QString> result;
    for (auto s : tags_vector) result.append(s);
    return result;
}

QList<QString> TagDialog::tagsToAdd() const
{
    auto originalTags = manager->tagsFor(note);
    auto currentTags = tags();
    return QSetToQList(
        QListToQSet(currentTags).subtract(QListToQSet(originalTags)));
}

QList<QString> TagDialog::tagsToRemove() const
{
    auto originalTags = manager->tagsFor(note);
    auto currentTags = tags();
    return QSetToQList(
        QListToQSet(originalTags).subtract(QListToQSet(currentTags)));
}

TagDialog::~TagDialog() { delete ui; }
