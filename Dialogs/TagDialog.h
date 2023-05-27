#ifndef TAGDIALOG_H
#define TAGDIALOG_H

#include <qlist.h>
#include <QDialog>
#include "NoteManager.h"

namespace Ui {
class TagDialog;
}

template <typename T>
QSet<T> QListToQSet(const QList<T> &qlist)
{
    return QSet<T>(qlist.constBegin(), qlist.constEnd());
}
template <typename T>
QList<T> QSetToQList(const QSet<T> &qlist)
{
    return QList<T>(qlist.constBegin(), qlist.constEnd());
}

class TagDialog : public QDialog
{
    Q_OBJECT

   public:
    explicit TagDialog(QWidget *parent = nullptr);
    explicit TagDialog(QWidget *parent = nullptr,
                       NoteManager *manager = nullptr, Note note = Note());

    void setCompletion(const QList<QString> &tags);

    QStringList tags() const;
    QList<QString> tagsToRemove() const;
    QList<QString> tagsToAdd() const;

    ~TagDialog();

   private:
    Ui::TagDialog *ui;
    NoteManager *manager;
    Note note;
};

#endif  // TAGDIALOG_H
