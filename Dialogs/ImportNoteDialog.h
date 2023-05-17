#ifndef IMPORTNOTEDIALOG_H
#define IMPORTNOTEDIALOG_H

#include <qlist.h>
#include <QDialog>
#include "NoteManager.h"

namespace Ui {
class ImportNoteDialog;
}

class ImportNoteDialog : public QDialog
{
    Q_OBJECT

   public:
    explicit ImportNoteDialog(QWidget *parent = nullptr);
    explicit ImportNoteDialog(QWidget *parent, NoteManager *manager);
    ~ImportNoteDialog();

    QStringList getPaths() const;
    QList<Note> getNotes() const;

   private:
    Ui::ImportNoteDialog *ui;
    NoteManager *manager;
};

#endif  // IMPORTNOTEDIALOG_H
