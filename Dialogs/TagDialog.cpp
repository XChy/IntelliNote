#include "TagDialog.h"
#include <qlist.h>
#include <qpushbutton.h>
#include <vector>
#include <QSet>
#include <QMessageBox>
#include "GPTSession.h"
#include "NoteManager.h"
#include "ui_TagDialog.h"

TagDialog::TagDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::TagDialog), gptSession(new GPTSession(this))
{
    ui->setupUi(this);
}

TagDialog::TagDialog(QWidget *parent, NoteManager *manager, Note note)
    : QDialog(parent),
      ui(new Ui::TagDialog),
      manager(manager),
      gptSession(new GPTSession(this)),
      note(note)
{
    ui->setupUi(this);

    auto tags = manager->tagsFor(note);
    std::vector<QString> currentTags(tags.begin(), tags.end());
    ui->tagEdit->tags(currentTags);

    setCompletion(manager->allTags());

    promptPattern = "请根据以下笔记内容给笔记打标签，标签之间用空格分隔：%1";

    connect(ui->autoTagButton, &QPushButton::pressed, this,
            &TagDialog::onAutoTag);

    connect(gptSession, &GPTSession::responseReceived,
            [this](const QString &response) {
                ui->autoTagButton->setEnabled(true);
                onCompleteAutoTag(response);
            });

    connect(gptSession, &GPTSession::errorOccured,
            [this](const QString &errorMessage) {
                QMessageBox::critical(
                    NULL, tr("Failed"),
                    tr("Failed to auto tag\n%1").arg(errorMessage));
                ui->autoTagButton->setEnabled(true);
            });
}

void TagDialog::onAutoTag()
{
    if (gptSession->askable()) {
        gptSession->ask(promptPattern.arg(manager->readNote(note)));
        ui->autoTagButton->setDisabled(true);
    } else {
        QMessageBox::critical(NULL, tr("Cannot auto tag"),
                              tr("GPT is still generating"));
    }
}

void TagDialog::onCompleteAutoTag(QString tags_str)
{
    auto qtagsList = tags_str.split(" ");
    std::vector<QString> tagsVector(qtagsList.begin(), qtagsList.end());
    ui->tagEdit->tags(tagsVector);
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
