#include "mainwindow.h"
#include <qabstractitemmodel.h>
#include <qaction.h>
#include <qmenu.h>
#include <qnamespace.h>
#include <qplaintextedit.h>
#include <qpoint.h>
#include <qstandarditemmodel.h>
#include <qstringliteral.h>
#include <qtextbrowser.h>
#include <QStandardPaths>
#include <QStandardItemModel>
#include <qmarkdowntextedit/markdownhighlighter.h>
#include <qtreeview.h>
#include "./ui_mainwindow.h"
#include "Dialogs/PromptGenerateDialog.h"
#include "GPTSession.h"
#include "NoteManager.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      generateDialog(new PromptGenerateDialog(this)),
      noteManager(new NoteManager(parent))
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

    QAction* copy_action = new QAction(tr("复制"));
    connect(copy_action, &QAction::triggered, ui->textEdit,
            &QPlainTextEdit::copy);

    QAction* paste_action = new QAction(tr("粘贴"));
    connect(paste_action, &QAction::triggered, ui->textEdit,
            &QPlainTextEdit::paste);

    QAction* cut_action = new QAction(tr("剪切"));
    connect(cut_action, &QAction::triggered, ui->textEdit,
            &QPlainTextEdit::cut);

    QAction* generate_content_action = new QAction(tr("生成内容（Markdown）"));
    connect(generate_content_action, &QAction::triggered, this,
            &MainWindow::onGenerateContent);

    QAction* generate_latex_action = new QAction(tr("生成内容（Latex）"));
    connect(generate_latex_action, &QAction::triggered, this,
            &MainWindow::onGenerateLatex);

    contextMenu->addActions({copy_action, paste_action, cut_action,
                             generate_content_action, generate_latex_action});

    connect(ui->textEdit, &QPlainTextEdit::customContextMenuRequested,
            [contextMenu, this](QPoint pos) {
                contextMenu->exec(ui->textEdit->mapToGlobal(pos));
            });

    // Note Manager
    noteManager->setNotesDirectory(
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
        "/IntelliNote");
    noteManager->readAll();

    model = new QStandardItemModel(this);
    setupModel();

    ui->noteManager->setModel(model);
    ui->noteManager->setEditTriggers(QTreeView::NoEditTriggers);
    ui->noteManager->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->noteManager->header()->setVisible(false);

    // sync the preview page with the editor
    connect(ui->textEdit, &QPlainTextEdit::textChanged,
            [=]() { ui->viewer->setText(ui->textEdit->toPlainText()); });
}

void MainWindow::onGenerateContent()
{
    generateDialog->clear();
    generateDialog->setPromptPattern("%1");
    if (generateDialog->exec() == QDialog::Accepted) {
        ui->textEdit->insertPlainText(generateDialog->getResult());
    }
}

void MainWindow::onGenerateLatex()
{
    generateDialog->clear();
    generateDialog->setPromptPattern(
        tr("Please generate pure latex code surrounded with $ without "
           "explanation as described "
           "below:\\n%1\\n ."));
    if (generateDialog->exec() == QDialog::Accepted) {
        ui->textEdit->insertPlainText(generateDialog->getResult());
    }
}

void MainWindow::setupModel()
{
    model->clear();
    QStandardItem* notebooks = new QStandardItem(tr("NoteBooks"));
    for (QString notebook : noteManager->allDirs()) {
        QStandardItem* notebook_item = new QStandardItem(notebook);
        for (Note note : noteManager->notesOfDir(notebook))
            notebook_item->appendRow(new QStandardItem(note.name));
        notebooks->appendRow(notebook_item);
    }

    QStandardItem* tags = new QStandardItem(tr("Tags"));
    for (QString tag : noteManager->allTags()) {
        QStandardItem* notebook_item = new QStandardItem(tag);
        for (Note note : noteManager->notesOfTag(tag))
            notebook_item->appendRow(new QStandardItem(note.name));
        notebooks->appendRow(notebook_item);
    }

    model->appendRow(notebooks);
    model->appendRow(tags);
}

MainWindow::~MainWindow() { delete ui; }
