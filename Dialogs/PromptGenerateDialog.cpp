#include "PromptGenerateDialog.h"
#include <qpushbutton.h>
#include <QMessageBox>
#include <QClipboard>>
#include "ui_PromptGenerateDialog.h"

PromptGenerateDialog::PromptGenerateDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::PromptGenerateDialog),
      gptSession(new GPTSession(this))
{
    ui->setupUi(this);
    connect(ui->generateButton, &QPushButton::pressed, this,
            &PromptGenerateDialog::onGenerate);

    connect(ui->stopButton, &QPushButton::pressed, this,
            &PromptGenerateDialog::onStop);

    connect(ui->copyButton, &QPushButton::pressed, [this](){
        QApplication::clipboard()->setText(result);
    });

    connect(gptSession, &GPTSession::responseReceived,
            [this](const QString &response) {
                result = response;
                ui->generateButton->setEnabled(true);
                ui->previewer->setText(response);
            });

    connect(gptSession, &GPTSession::errorOccured,
            [this](const QString &errorMessage) {
                result = "";
                QMessageBox::critical(
                    NULL, tr("Failed"),
                    tr("Failed to generate\n%1").arg(errorMessage));
                ui->generateButton->setEnabled(true);
            });
}

void PromptGenerateDialog::onGenerate()
{
    if (gptSession->askable()) {
        gptSession->ask(promptPattern.arg(ui->plainTextEdit->toPlainText()));
        ui->generateButton->setDisabled(true);
    } else {
        QMessageBox::critical(NULL, tr("Cannot generate"),
                              tr("GPT is still generating"));
    }
}

void PromptGenerateDialog::onStop()
{
    gptSession->stopAsk();
    ui->generateButton->setEnabled(true);
}

void PromptGenerateDialog::setPromptPattern(const QString &prompt)
{
    this->promptPattern = prompt;
}

QString PromptGenerateDialog::getPromptPattern() const { return promptPattern; }

QString PromptGenerateDialog::getResult() const { return result; }

void PromptGenerateDialog::clear() { ui->plainTextEdit->clear(); }

PromptGenerateDialog::~PromptGenerateDialog() { delete ui; }
