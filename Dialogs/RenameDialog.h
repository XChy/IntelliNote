#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>

namespace Ui {
class RenameDialog;
}

class RenameDialog : public QDialog
{
    Q_OBJECT

   public:
    explicit RenameDialog(QWidget *parent = nullptr);
    void setOldName(const QString &oldName);
    QString getNewName() const;

    ~RenameDialog();

   private:
    Ui::RenameDialog *ui;
};

#endif  // RENAMEDIALOG_H
