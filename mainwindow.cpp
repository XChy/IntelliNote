#include "mainwindow.h"
#include <qaction.h>
#include <qmenu.h>
#include <qnamespace.h>
#include <qplaintextedit.h>
#include <qpoint.h>
#include <qtextbrowser.h>
#include <qtextdocument.h>
#include <QStandardItemModel>
#include <qmarkdowntextedit/markdownhighlighter.h>
#include <qtreeview.h>
#include "./ui_mainwindow.h"
#include "Dialogs/PromptGenerateDialog.h"
#include "GPTSession.h"

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

    ui->textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    QMenu* contextMenu = new QMenu(ui->textEdit);

    QAction* copy_action = new QAction(tr("Copy"));
    connect(copy_action, &QAction::triggered, ui->textEdit,
            &QPlainTextEdit::copy);

    QAction* paste_action = new QAction(tr("paste"));
    connect(paste_action, &QAction::triggered, ui->textEdit,
            &QPlainTextEdit::paste);

    QAction* cut_action = new QAction(tr("cut"));
    connect(cut_action, &QAction::triggered, ui->textEdit,
            &QPlainTextEdit::cut);

    QAction* generate_content_action = new QAction(tr("Generate content for"));
    connect(generate_content_action, &QAction::triggered, this,
            &MainWindow::onGenerateContent);

    QAction* generate_latex_action = new QAction(tr("Generate latex for"));
    connect(generate_content_action, &QAction::triggered, this,
            &MainWindow::onGenerateLatex);

    contextMenu->addActions({copy_action, paste_action, cut_action,
                             generate_content_action, generate_latex_action});

    connect(ui->textEdit, &QPlainTextEdit::customContextMenuRequested,
            [contextMenu, this](QPoint pos) {
                contextMenu->exec(ui->textEdit->mapToGlobal(pos));
            });

    // Note Manager
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

void MainWindow::onGenerateContent()
{
    PromptGenerateDialog* dialog = new PromptGenerateDialog(this);
    dialog->setPromptPattern("%1");
    int result = dialog->exec();
    if (result == QDialog::Accepted) {
        ui->textEdit->insertPlainText(dialog->getResult());
    }
}

void MainWindow::onGenerateLatex() {}

MainWindow::~MainWindow() { delete ui; }
