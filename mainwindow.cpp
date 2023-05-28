#include "mainwindow.h"
#include <qaction.h>
#include <QScrollBar>
#include <QMessageBox>
#include <qdialog.h>
#include <qevent.h>
#include <qfont.h>
#include <qkeysequence.h>
#include <qlist.h>
#include <qmessagebox.h>
#include <QFileDialog>
#include <qnamespace.h>
#include <qplaintextedit.h>
#include <qpoint.h>
#include <qscrollbar.h>
#include <qstandarditemmodel.h>
#include <qstringliteral.h>
#include <qtextbrowser.h>
#include <QStandardPaths>
#include <QStandardItemModel>
#include <ios>
#include <qmarkdowntextedit/markdownhighlighter.h>
#include <qtextedit.h>
#include <qtmetamacros.h>
#include <qtreeview.h>
#include "./ui_mainwindow.h"
#include "Dialogs/AboutDialog.h"
#include "Dialogs/NewNoteBookDialog.h"
#include "Dialogs/PromptGenerateDialog.h"
#include "Dialogs/RenameDialog.h"
#include "Dialogs/SummaryDialog.h"
#include "Dialogs/TagDialog.h"
#include "NoteManager.h"
#include <QFile>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      noteManager(new NoteManager(parent)),
      generateDialog(new PromptGenerateDialog(this)),
      newNoteDialog(new NewNoteDialog(this, noteManager)),
      importNoteDialog(new ImportNoteDialog(this, noteManager)),
      model(new QStandardItemModel(this)),
      notebooks_item(new QStandardItem(tr("NoteBooks"))),
      tags_item(new QStandardItem(tr("Tags")))
{
    QFile f(":qdarkstyle/dark/darkstyle.qss");
    QFile f2(":qlightstyle/light/lightstyle.qss");
    QFile f3("://qlightstyle/trial.qss");
    f.open(QFile::ReadOnly );
    setStyleSheet(f.readAll());
    f.close();
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 3);

    // highlight the markdown editor
    auto doc = ui->textEdit->document();
    auto* highlighter = new MarkdownHighlighter(doc);

    // context menu for editor
    ui->textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    QMenu* menuForEdittor = new QMenu(ui->textEdit);

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

    QAction* generate_outline_action = new QAction(tr("生成大纲"));
    connect(generate_outline_action, &QAction::triggered, this,
            &MainWindow::onGenerateOutline);

    QAction* generate_summary_action = new QAction(tr("生成总结"));
    connect(generate_summary_action, &QAction::triggered, this,
            &MainWindow::onGenerateSummary);

    menuForEdittor->addActions({copy_action, paste_action, cut_action,
                                generate_content_action, generate_latex_action,
                                generate_outline_action,
                                generate_summary_action});

    // context menu for the note manager
    menuForManager = new QMenu(ui->noteManager);
    connect(ui->noteManager, &QTreeView::customContextMenuRequested, this,
            &MainWindow::onMenuForManager);

    // textEdit
    connect(ui->textEdit, &QPlainTextEdit::customContextMenuRequested,
            [menuForEdittor, this](QPoint pos) {
                menuForEdittor->exec(ui->textEdit->mapToGlobal(pos));
            });

    connect(ui->textEdit, &QPlainTextEdit::modificationChanged,
            [this](bool isModified) {
                if (isModified)
                    ui->nameLabel->setText(currentNote.name + " *");
                else
                    ui->nameLabel->setText(currentNote.name);
            });

    ui->textEdit->installEventFilter(this);

    // topbox
    connect(ui->actionNewNote, &QAction::triggered, this,
            &MainWindow::onNewNote);

    connect(ui->actionSaveNote, &QAction::triggered, [this]() {
        if (!currentNote.name.isEmpty()) {
            noteManager->saveNote(currentNote, ui->textEdit->toPlainText());
            ui->textEdit->setPlainText(ui->textEdit->toPlainText());
        }
    });

    connect(ui->actionImportNote, &QAction::triggered, this,
            &MainWindow::onImportNote);

    connect(ui->actionMarkdown, &QAction::triggered, [this]() {
        QString path = QFileDialog::getSaveFileName(this, tr("导出为Markdown"));
        QFile file(path);
        file.open(QFile::WriteOnly);
        file.write(ui->textEdit->toPlainText().toLocal8Bit());
        file.close();
    });

    connect(ui->actionHTML, &QAction::triggered, [this]() {

    });

    connect(ui->actionPDF, &QAction::triggered, [this]() {
        QString path = QFileDialog::getSaveFileName(this, tr("导出为PDF"));
        ui->viewer->printToPdf(path);
    });

    connect(ui->actionAbout, &QAction::triggered, [this]() {});

    // Note Manager
    currentNote.type = Note::NoNote;
    noteManager->setNotesDirectory(
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
        "/IntelliNote");

    connect(noteManager, &NoteManager::noteChanged, this,
            &MainWindow::setupModel);
    noteManager->readAll();

    ui->noteManager->setModel(model);
    ui->noteManager->setEditTriggers(QTreeView::NoEditTriggers);
    ui->noteManager->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->noteManager->header()->setVisible(false);

    // Model
    connect(ui->noteManager, &QTreeView::doubleClicked, this,
            &MainWindow::onOpen);

    // sync the preview page with the editor
    connect(ui->textEdit, &QPlainTextEdit::textChanged,
            [=]() { ui->viewer->setText(ui->textEdit->toPlainText()); });

    connect(
        ui->textEdit->verticalScrollBar(), &QScrollBar::valueChanged,
        [this](int value) {
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
           "below:\n%1"));
    if (generateDialog->exec() == QDialog::Accepted) {
        ui->textEdit->insertPlainText(generateDialog->getResult());
    }
}

void MainWindow::onGenerateOutline()
{
    generateDialog->clear();
    generateDialog->setPromptPattern(
        tr("Please generate an outline in markdown without "
           "explanation as described "
           "below:\n%1"));
    if (generateDialog->exec() == QDialog::Accepted) {
        ui->textEdit->insertPlainText(generateDialog->getResult());
    }
}

void MainWindow::onGenerateSummary()
{
    QString text;
    if (ui->textEdit->textCursor().selectedText().isEmpty()) {
        text = ui->textEdit->toPlainText();
    } else {
        text = ui->textEdit->textCursor().selectedText();
    }

    SummaryDialog dialog;
    dialog.setText(text);
    dialog.onGenerate();
    dialog.exec();
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
            for (auto tag : newNoteDialog->tags()) {
                noteManager->tagNote(note, tag);
            }
            emit noteManager->noteChanged();
            switchNote(note);
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
                    NULL, tr("Cannot import note:%1").arg(paths[i]),
                    tr("There is an note with the same name"));
            } else if (err == 2) {
                QMessageBox::critical(
                    NULL, tr("Cannot import note:%1").arg(paths[i]),
                    tr("Some errors occurs in the filesystem"));
            }
        }
    }
}

void MainWindow::onOpen(const QModelIndex& index)
{
    auto item = model->itemFromIndex(index);
    if (isNoteItem(index)) {
        switchNote(noteForIndex(index));
    }
}

void MainWindow::onMenuForManager(const QPoint& pos)
{
    menuForManager->clear();
    QModelIndex curIndex = ui->noteManager->indexAt(pos);

    if (curIndex.isValid()) {
        if (isNoteItem(curIndex)) {
            menuForManager->addAction(tr("Remove"), [this, curIndex]() {
                noteManager->removeNote(noteForIndex(curIndex));
            });

            menuForManager->addAction(tr("Rename"), [this, curIndex]() {
                // TODO: rename dialog
                RenameDialog dialog;
                dialog.setOldName(noteForIndex(curIndex).name);
                if (dialog.exec() == QDialog::Accepted) {
                    int err = noteManager->renameNote(noteForIndex(curIndex),
                                                      dialog.getNewName());
                }
            });

            menuForManager->addAction(tr("Tag"), [this, curIndex]() {
                TagDialog dialog(nullptr, noteManager, noteForIndex(curIndex));
                if (dialog.exec() == QDialog::Accepted) {
                    auto tagsToAdd = dialog.tagsToAdd();
                    auto tagsToRemove = dialog.tagsToRemove();
                    for (auto tag : tagsToAdd)
                        noteManager->tagNote(noteForIndex(curIndex), tag);
                    for (auto tag : tagsToRemove)
                        noteManager->untagNote(noteForIndex(curIndex), tag);
                    emit noteManager->noteChanged();
                }
            });

        } else if (isNoteBookItem(curIndex)) {
            menuForManager->addAction(tr("New Note"), [this, curIndex]() {
                newNoteDialog->setCurrentDir(textForIndex(curIndex));
                onNewNote();
            });

            menuForManager->addAction(
                tr("Remove this notebook"), [this, curIndex]() {
                    // TODO: Question Message Box
                    noteManager->removeDir(textForIndex(curIndex));
                });

        } else if (isTagItem(curIndex)) {
            menuForManager->addAction(tr("New Note"), [this, curIndex]() {
                newNoteDialog->setCurrentDir(textForIndex(curIndex));
                onNewNote();
            });

            menuForManager->addAction(tr("Remove"), [this, curIndex]() {
                // TODO: Question Message Box
                noteManager->removeDir(textForIndex(curIndex));
            });
        } else if (isNoteBooksItem(curIndex)) {
            menuForManager->addAction(tr("New Notebook"), [this, curIndex]() {
                NewNoteBookDialog dialog(this);
                if (dialog.exec() == QDialog::Accepted) {
                    int err = noteManager->createDir(dialog.notebook());
                    if (err != 0) {
                        QMessageBox::critical(this,
                                              tr("Fail creating notebook"),
                                              tr("Fail creating notebook"));
                    }
                }
            });

            menuForManager->addAction(tr("New Note"), [this, curIndex]() {
                newNoteDialog->setCurrentDir(textForIndex(curIndex));
                onNewNote();
            });
        }

        menuForManager->exec(ui->noteManager->mapToGlobal(pos));
    }
}

bool MainWindow::eventFilter(QObject* w, QEvent* e)
{
    // handle ctrl + s
    if (e->type() == QEvent::KeyPress) {
        QKeyEvent* key_e = (QKeyEvent*)e;
        if (key_e->modifiers() == Qt::ControlModifier &&
            key_e->key() == Qt::Key_S) {
            noteManager->saveNote(currentNote, ui->textEdit->toPlainText());
            ui->textEdit->setPlainText(ui->textEdit->toPlainText());
            return true;
        }
    }
    return false;
}

void MainWindow::switchNote(const Note& note)
{
    if (note.name == currentNote.name && note.dir == currentNote.dir) return;

    if (ui->textEdit->document()->isModified()) {
        if (QMessageBox::question(
                NULL, tr("One note remains unsaved"),
                tr("Do you want to save the note %1 ?").arg(currentNote.name),
                QMessageBox::Save | QMessageBox::Discard) ==
            QMessageBox::Save) {
            noteManager->saveNote(currentNote, ui->textEdit->toPlainText());
        }
    }

    currentNote = note;
    if (note.type != Note::NoNote) {
        ui->textEdit->setPlainText(noteManager->readNote(note));
        ui->nameLabel->setText(note.name);
    } else {
        ui->textEdit->setPlainText("");
        ui->nameLabel->setText(tr("未命名"));
    }
}

bool MainWindow::isNoteItem(const QModelIndex& index)
{
    auto item = model->itemFromIndex(index);
    return item->parent() && item->parent()->parent();
}

bool MainWindow::isNoteBookItem(const QModelIndex& index)
{
    auto item = model->itemFromIndex(index);
    return item->parent() && item->parent() == notebooks_item;
}
bool MainWindow::isNoteBooksItem(const QModelIndex& index)
{
    auto item = model->itemFromIndex(index);
    return item == notebooks_item;
}

bool MainWindow::isTagItem(const QModelIndex& index)
{
    auto item = model->itemFromIndex(index);
    return item->parent() && item->parent() == tags_item;
}

bool MainWindow::isTagsItem(const QModelIndex& index)
{
    auto item = model->itemFromIndex(index);
    return item == tags_item;
}

Note MainWindow::noteForIndex(const QModelIndex& index)
{
    auto item = model->itemFromIndex(index);
    if (item->parent()->parent() == notebooks_item) {
        Note note;
        note.dir = item->parent()->text();
        note.name = item->text();
        note.path = noteManager->pathForInternal(note);
        return note;
    } else if (item->parent()->parent() == tags_item) {
        QString tag = item->parent()->text();
        auto notes = noteManager->notesOfTag(tag);
        for (auto note : notes) {
            if (note.name == item->text()) {
                return note;
            }
        }
    }

    return Note();
}

QString MainWindow::textForIndex(const QModelIndex& index)
{
    return model->itemFromIndex(index)->text();
}

void MainWindow::setupModel()
{
    model->clear();

    auto font = QFont();
    font.setPointSize(14);
    font.setBold(true);

    notebooks_item = new QStandardItem(tr("Notebooks"));
    tags_item = new QStandardItem(tr("Tags"));

    notebooks_item->setFont(font);
    tags_item->setFont(font);

    for (QString notebook : noteManager->allDirs()) {
        QStandardItem* notebook_item = new QStandardItem(notebook);
        for (Note note : noteManager->notesOfDir(notebook))
            notebook_item->appendRow(new QStandardItem(note.name));
        notebooks_item->appendRow(notebook_item);
    }

    for (QString tag : noteManager->allTags()) {
        QStandardItem* notebook_item = new QStandardItem(tag);
        for (Note note : noteManager->notesOfTag(tag))
            notebook_item->appendRow(new QStandardItem(note.name));
        tags_item->appendRow(notebook_item);
    }

    model->appendRow(notebooks_item);
    model->appendRow(tags_item);

    ui->noteManager->expand(notebooks_item->index());
    ui->noteManager->expand(tags_item->index());
}

MainWindow::~MainWindow() { delete ui; }
