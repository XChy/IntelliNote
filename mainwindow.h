#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Dialogs/PromptGenerateDialog.h"
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

   private:
    Ui::MainWindow *ui;

    PromptGenerateDialog *generateDialog;
    NoteManager *noteManager;
};
#endif  // MAINWINDOW_H
