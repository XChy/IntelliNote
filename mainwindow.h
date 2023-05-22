#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qabstractitemmodel.h>
#include <qcoreevent.h>
#include <qmenu.h>
#include <qstandarditemmodel.h>
#include <QMainWindow>
#include "Dialogs/PromptGenerateDialog.h"
#include "Dialogs/NewNoteDialog.h"
#include "Dialogs/ImportNoteDialog.h"
#include "Note.h"
#include "NoteManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void onGenerateContent();
    void onGenerateLatex();

    void onNewNote();
    void onImportNote();

    void onOpen(const QModelIndex &index);

    void onMenuForManager(const QPoint &pos);

    bool eventFilter(QObject *w, QEvent *e);

    void switchNote(const Note &note);

    bool isNoteItem(const QModelIndex &index);
    bool isNoteBookItem(const QModelIndex &index);
    bool isNoteBooksItem(const QModelIndex &index);
    bool isTagItem(const QModelIndex &index);
    bool isTagsItem(const QModelIndex &index);
    QString textForIndex(const QModelIndex &index);
    Note noteForIndex(const QModelIndex &index);

    void setupModel();

   private:
    Ui::MainWindow *ui;

    NoteManager *noteManager;

    PromptGenerateDialog *generateDialog;
    NewNoteDialog *newNoteDialog;
    ImportNoteDialog *importNoteDialog;

    QMenu *menuForManager;

    QStandardItemModel *model;

    QStandardItem *notebooks_item;
    QStandardItem *tags_item;

    Note currentNote;
};
#endif  // MAINWINDOW_H
