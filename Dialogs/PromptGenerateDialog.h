#ifndef PROMPTGENERATEDIALOG_H
#define PROMPTGENERATEDIALOG_H

#include <QDialog>
#include "GPTSession.h"

namespace Ui {
class PromptGenerateDialog;
}

class PromptGenerateDialog : public QDialog
{
    Q_OBJECT

   public:
    explicit PromptGenerateDialog(QWidget *parent = nullptr);
    ~PromptGenerateDialog();

    void onGenerate();
    void onStop();

    void setPromptPattern(const QString &prompt);
    QString getPromptPattern() const;

    QString getResult() const;

    void clear();

   private:
    Ui::PromptGenerateDialog *ui;
    GPTSession *gptSession;
    QString promptPattern;
    QString result;
};

#endif  // PROMPTGENERATEDIALOG_H
