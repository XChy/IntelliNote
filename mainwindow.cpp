#include "mainwindow.h"
#include <qabstractitemmodel.h>
#include <qaction.h>
#include <QScrollBar>
#include <QMessageBox>
#include <qlist.h>
#include <qpoint.h>
#include <qscrollbar.h>
#include <qstandarditemmodel.h>
#include <qstringliteral.h>
#include <qtextbrowser.h>
#include <QStandardPaths>
#include <QStandardItemModel>
#include <qmarkdowntextedit/markdownhighlighter.h>
#include <qtextedit.h>
#include <qtreeview.h>
#include "./ui_mainwindow.h"
#include "Dialogs/PromptGenerateDialog.h"
#include "GPTSession.h"
#include "NoteManager.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      noteManager(new NoteManager(parent)),
      generateDialog(new PromptGenerateDialog(this)),
      newNoteDialog(new NewNoteDialog(this, noteManager)),
      importNoteDialog(new ImportNoteDialog(this, noteManager))
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 2);
    ui->splitter->setStretchFactor(2, 4);

    // highlight the markdown editor
    auto doc = ui->textEdit->document();
    auto* highlighter = new MarkdownHighlighter(doc);

    // context menu for editor
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

    // topbox
    connect(ui->actionNewNote, &QAction::triggered, this,
            &MainWindow::onNewNote);
    connect(ui->actionImportNote, &QAction::triggered, this,
            &MainWindow::onImportNote);

    // Note Manager
    noteManager->setNotesDirectory(
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
        "/IntelliNote");

    model = new QStandardItemModel(this);

    connect(noteManager, &NoteManager::noteChanged, this,
            &MainWindow::setupModel);
    noteManager->readAll();

    ui->noteManager->setModel(model);
    ui->noteManager->setEditTriggers(QTreeView::NoEditTriggers);
    ui->noteManager->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->noteManager->header()->setVisible(false);

    connect(noteManager, &NoteManager::noteChanged, this,
            &MainWindow::setupModel);

    // sync the preview page with the editor
    connect(ui->textEdit, &QPlainTextEdit::textChanged,
            [=]() { ui->viewer->setText(ui->textEdit->toPlainText()); });
    connect(
        ui->textEdit->verticalScrollBar(), &QScrollBar::valueChanged,
        [this](int value) {
            qDebug() << value;
            ui->viewer->page()->runJavaScript(
                QStringLiteral(
                    "window.scrollTo(0 ,document.body.clientHeight * %1)")
                    .arg(double(value) /
                         double(ui->textEdit->verticalScrollBar()->maximum())));
        });
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

void MainWindow::onNewNote()
{
    if (newNoteDialog->exec() == QDialog::Accepted) {
        Note note = newNoteDialog->getNote();
        int err = noteManager->createNote(note);

        if (err == 1) {
            QMessageBox::critical(NULL, tr("Cannot create new note"),
                                  tr("There is an note with the same name"));
        } else if (err == 2) {
            QMessageBox::critical(NULL, tr("Cannot create new note"),
                                  tr("Some errors occurs in the filesystem"));
        } else {
            QMessageBox::information(NULL, tr("Succeed creating new note"),
                                     tr("Succeed creating new note"));
        }
    }
}

void MainWindow::onImportNote()
{
    if (importNoteDialog->exec() == QDialog::Accepted) {
        QStringList paths = importNoteDialog->getPaths();
        QList<Note> notes = importNoteDialog->getNotes();
        for (int i = 0; i < notes.size(); ++i) {
            int err = noteManager->importNote(notes[i], paths[i]);

            // TODO: error handling
            if (err == 1) {
                QMessageBox::critical(
                    NULL, tr("Cannot import note"),
                    tr("There is an note with the same name"));
            } else if (err == 2) {
                QMessageBox::critical(
                    NULL, tr("Cannot import note"),
                    tr("Some errors occurs in the filesystem"));
            }
        }
    }
    QDialog Dialogs;
    Note note;
    QString note_path;
    noteManager->importNote(note, note_path);
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
