#include "mainwindow.h"
#include <qnamespace.h>
#include <qtextbrowser.h>
#include <qtextdocument.h>
#include <qtextedit.h>
#include <qtextformat.h>
#include <QStandardItemModel>
#include <qmarkdowntextedit/markdownhighlighter.h>
#include <qtreeview.h>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 2);
    ui->splitter->setStretchFactor(2, 4);

    // highlight the markdown editor
    auto doc = ui->textEdit->document();
    auto* highlighter = new MarkdownHighlighter(doc);

    auto model = new QStandardItemModel;
    model->appendRow(new QStandardItem("asd"));
    model->item(0)->setChild(0, 0, new QStandardItem("asd"));

    ui->noteManager->setModel(model);
    ui->noteManager->setEditTriggers(QTreeView::NoEditTriggers);
    ui->noteManager->setContextMenuPolicy(Qt::CustomContextMenu);

    // sync the preview page with the editor
    connect(ui->textEdit, &QPlainTextEdit::textChanged,
            [=]() { ui->viewer->setText(ui->textEdit->toPlainText()); });
}

MainWindow::~MainWindow() { delete ui; }
