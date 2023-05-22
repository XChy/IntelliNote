#ifndef NEWNOTEBOOKDIALOG_H
#define NEWNOTEBOOKDIALOG_H

#include <QDialog>

namespace Ui {
class NewNoteBookDialog;
}

class NewNoteBookDialog : public QDialog
{
    Q_OBJECT

   public:
    explicit NewNoteBookDialog(QWidget *parent = nullptr);
    ~NewNoteBookDialog();

    QString notebook() const;

   private:
    Ui::NewNoteBookDialog *ui;
};

#endif  // NEWNOTEBOOKDIALOG_H
