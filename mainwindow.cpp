#include "mainwindow.h"
#include <qtextbrowser.h>
#include <qtextdocument.h>
#include <qtextedit.h>
#include <qtextformat.h>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTextTableFormat format;
    format.setBorderCollapse(true);
    format.setCellPadding(21.2);
    ui->textBrowser->mergeCurrentCharFormat(format.toCharFormat());

    connect(ui->textEdit, &QTextEdit::textChanged, [=]() {
        ui->textBrowser->setMarkdown(ui->textEdit->toPlainText());
    });
}

MainWindow::~MainWindow() { delete ui; }
