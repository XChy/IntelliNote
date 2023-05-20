#ifndef NEWNOTEDIALOG_H
#define NEWNOTEDIALOG_H

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

   private:
    Ui::NewNoteDialog *ui;
    Note newNote;
    NoteManager *manager;
};

#endif  // NEWNOTEDIALOG_H
