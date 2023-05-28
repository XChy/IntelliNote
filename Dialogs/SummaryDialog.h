#ifndef SUMMARYDIALOG_H
#define SUMMARYDIALOG_H

#include <qstyle.h>
#include <QDialog>
#include "GPTSession.h"

namespace Ui {
class SummaryDialog;
}

class SummaryDialog : public QDialog
{
    Q_OBJECT

   public:
    explicit SummaryDialog(QWidget *parent = nullptr);
    ~SummaryDialog();

    void onGenerate();
    void onStop();

    void setText(const QString &text);

    void setPromptPattern(const QString &prompt);
    QString getPromptPattern() const;

    QString getResult() const;

   private:
    GPTSession *gptSession;
    QString promptPattern;
    QString result;
    QString text;

   private:
    Ui::SummaryDialog *ui;
};

#endif  // SUMMARYDIALOG_H
