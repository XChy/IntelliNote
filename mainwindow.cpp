#include "mainwindow.h"
#include <qtextbrowser.h>
#include <qtextdocument.h>
#include <qtextedit.h>
#include <qtextformat.h>
#include <qmarkdowntextedit/markdownhighlighter.h>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,2);
    ui->splitter->setStretchFactor(2,4);

    auto doc = ui->textEdit->document();
    auto* highlighter = new MarkdownHighlighter(doc);

    connect(ui->textEdit, &QPlainTextEdit::textChanged, [=]() {
        ui->viewer->setText(ui->textEdit->toPlainText());
    });
}

MainWindow::~MainWindow() { delete ui; }
