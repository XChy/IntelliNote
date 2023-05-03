#include "mainwindow.h"
#include <qtextdocument.h>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->textEdit, &QTextEdit::textChanged, [=]() {
        ui->textBrowser->setMarkdown(ui->textEdit->toPlainText());
    });
}

MainWindow::~MainWindow() { delete ui; }
