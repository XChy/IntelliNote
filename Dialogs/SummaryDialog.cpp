#include <QClipboard>
#include <QMessageBox>
#include "SummaryDialog.h"
#include "ui_SummaryDialog.h"

SummaryDialog::SummaryDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::SummaryDialog),
      gptSession(new GPTSession(this))
{
    ui->setupUi(this);
    connect(ui->copyButton, &QPushButton::pressed,
            [this]() { QApplication::clipboard()->setText(result); });

    connect(gptSession, &GPTSession::responseReceived,
            [this](const QString &response) {
                result = response;
                ui->viewer->setText(response);
            });

    connect(gptSession, &GPTSession::errorOccured,
            [this](const QString &errorMessage) {
                result = "";
                QMessageBox::critical(
                    NULL, tr("Failed"),
                    tr("Failed to generate\n%1").arg(errorMessage));
            });
    promptPattern =
        R"(请帮我总结一下这一段笔记内容，要求是只要你的总结内容，不需要其他内容，概括尽可能简介，字数在50-100左右，可以根据文本长度微调，并且以markdown格式呈现："%1")";
}

void SummaryDialog::onGenerate()
{
    if (gptSession->askable()) {
        gptSession->ask(promptPattern.arg(text));
    } else {
        QMessageBox::critical(NULL, tr("Cannot generate"),
                              tr("GPT is still generating"));
    }
}

void SummaryDialog::onStop() { gptSession->stopAsk(); }

void SummaryDialog::setText(const QString &text) { this->text = text; }

SummaryDialog::~SummaryDialog() { delete ui; }
