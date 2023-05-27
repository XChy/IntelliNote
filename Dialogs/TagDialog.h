#ifndef TAGDIALOG_H
#define TAGDIALOG_H

#include <qlist.h>
#include <QDialog>
#include "NoteManager.h"

namespace Ui {
class TagDialog;
}

class TagDialog : public QDialog
{
    Q_OBJECT

   public:
    explicit TagDialog(QWidget *parent = nullptr);
    explicit TagDialog(QWidget *parent = nullptr,
                       NoteManager *manager = nullptr, Note note = Note());

    void setOriginalTags(const QList<QString> &tags);

    QList<QString> tags() const;
    QList<QString> tagsToRemove() const;
    QList<QString> tagsToAdd() const;

    ~TagDialog();

   private:
    Ui::TagDialog *ui;
    NoteManager *manager;
    Note note;
};

#endif  // TAGDIALOG_H
