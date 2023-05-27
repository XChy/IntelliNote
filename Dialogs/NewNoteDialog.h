#ifndef NEWNOTEDIALOG_H
#define NEWNOTEDIALOG_H

#include <qlist.h>
#include <QDialog>
#include "Note.h"
#include "NoteManager.h"

namespace Ui {
class NewNoteDialog;
}

class NewNoteDialog : public QDialog
{
    Q_OBJECT

   public:
    explicit NewNoteDialog(QWidget *parent = nullptr);
    explicit NewNoteDialog(QWidget *parent, NoteManager *manager);
    ~NewNoteDialog();

    Note getNote() const;
    QList<QString> tags() const;

    void setCurrentDir(const QString &notebook);

   private:
    Ui::NewNoteDialog *ui;
    Note newNote;
    NoteManager *manager;
};

#endif  // NEWNOTEDIALOG_H
