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

    connect(ui->textEdit, &QTextEdit::textChanged, [=]() {
        ui->viewer->setText(ui->textEdit->toPlainText());
    });
}

MainWindow::~MainWindow() { delete ui; }
