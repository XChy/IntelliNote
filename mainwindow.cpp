#include "mainwindow.h"
#include <qglobal.h>
#include <qobjectdefs.h>
#include <qtextbrowser.h>
#include <qtextedit.h>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->textEdit, &QTextEdit::textChanged, [=]() {
        ui->textBrowser->setMarkdown(ui->textEdit->toMarkdown());
    });
}

MainWindow::~MainWindow() { delete ui; }
