
#include "mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QSize>
#include <QTextStream>
#include <QToolBar>

#include "texteditor.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->setupUi(this);

  ToolBar = new QToolBar;
  ToolBar->addAction(ui->actionNew);
  ToolBar->addAction(ui->actionOpen);
  ToolBar->addAction(ui->actionSave);
  ToolBar->addAction(ui->actionSave_As);
  ToolBar->addAction(ui->actionUndo);
  ToolBar->addAction(ui->actionRedo);
  ToolBar->addAction(ui->actionQuit);
  ToolBar->addAction(ui->actionAbout);

  QSize *qs = new QSize;
  ToolBar->setIconSize(qs->scaled(26, 26, Qt::IgnoreAspectRatio));
  this->addToolBar(Qt::LeftToolBarArea, ToolBar);

  plainTextEdit = new TextEditor;
  this->setCentralWidget(plainTextEdit);

  setWindowTitle("Notepanda");

  connect(ui->actionNew, &QAction::triggered, plainTextEdit,
          &TextEditor::newDocument);
  connect(ui->actionOpen, &QAction::triggered, plainTextEdit,
          &TextEditor::open);
  connect(ui->actionSave, &QAction::triggered, plainTextEdit,
          &TextEditor::save);
  connect(ui->actionSave_As, &QAction::triggered, plainTextEdit,
          &TextEditor::saveAs);
  connect(ui->actionUndo, &QAction::triggered, plainTextEdit,
          &TextEditor::undo);
  connect(ui->actionRedo, &QAction::triggered, plainTextEdit,
          &TextEditor::redo);
  connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quit);
  connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
  connect(plainTextEdit, &TextEditor::changeTitle, this,
          &MainWindow::changeWindowTitle);
  connect(plainTextEdit, &TextEditor::undoAvailable, this,
          &MainWindow::setActUndoState);
  connect(plainTextEdit, &TextEditor::redoAvailable, this,
          &MainWindow::setActRedoState);

#if !QT_CONFIG(clipboard)
  ui->actionCut->setEnabled(false);
  ui->actionCopy->setEnabled(false);
  ui->actionPaste->setEnabled(false);
#else
  connect(ui->actionCopy, &QAction::triggered, plainTextEdit,
          &TextEditor::copy);
  connect(ui->actionPaste, &QAction::triggered, plainTextEdit,
          &TextEditor::paste);
  connect(ui->actionCut, &QAction::triggered, plainTextEdit, &TextEditor::cut);
  connect(plainTextEdit, &TextEditor::copyAvailable, this,
          &MainWindow::setActCopyState);
  connect(plainTextEdit, &TextEditor::copyAvailable, this,
          &MainWindow::setActCutState);
#endif
}

MainWindow::~MainWindow()
{
  delete ui;
  delete plainTextEdit;
}

void MainWindow::about()
{
  QMessageBox::about(
      this, tr("About Notepanda"),
      tr("<center><b>Notepanda</b></center> <br> <center>A simple "
         "cross-platform notepad. Based on Qt and C++.</center><br>"
         "<b>Vesrion:</b> 0.0.1-alpha<br>"
         "Copyright © 2020 ChungZH & Shawfing."));
}

void MainWindow::changeWindowTitle()
{
  if (!plainTextEdit->currentFile.isEmpty())
    setWindowTitle(plainTextEdit->currentFile.split("/").last() +
                   " - Notepanda");
  else
    setWindowTitle(tr("Untitled") + " - Notepanda");
}

void MainWindow::setActUndoState(bool available)
{
  ui->actionUndo->setDisabled(!available);
}

void MainWindow::setActRedoState(bool available)
{
  ui->actionRedo->setDisabled(!available);
}

void MainWindow::setActCopyState(bool available)
{
  ui->actionCopy->setDisabled(!available);
}

void MainWindow::setActCutState(bool available)
{
  ui->actionCut->setDisabled(!available);
}

void MainWindow::quit() { QCoreApplication::quit(); }
