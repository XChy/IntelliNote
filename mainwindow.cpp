#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textBrowser->setMarkdown("## XChy\n### abc\nas\n### b\nasdsad\n```C\nint a = 3\n```");
}

MainWindow::~MainWindow()
{
    delete ui;
}

