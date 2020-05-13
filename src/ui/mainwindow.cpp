#include "mainwindow.h"

#include <QDebug>
#include <QSize>
#include <QStyle>
#include <QStyleFactory>
#include <QTextStream>
#include <QToolBar>

#include "aboutwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  configManager = new ConfigManager;
  QApplication::setStyle(QStyleFactory::create(configManager->getStyle()));

  ToolBar = new QToolBar;
  ToolBar->addAction(ui->actionNew);
  ToolBar->addAction(ui->actionOpen);
  ToolBar->addAction(ui->actionSave);
  ToolBar->addAction(ui->actionSave_As);
  ToolBar->addAction(ui->actionPreferences);
  ToolBar->addAction(ui->actionPrint);
  ToolBar->addAction(ui->actionUndo);
  ToolBar->addAction(ui->actionRedo);
  ToolBar->addAction(ui->actionQuit);
  ToolBar->addAction(ui->actionAbout);

  QSize *qs = new QSize;
  ToolBar->setIconSize(qs->scaled(26, 26, Qt::IgnoreAspectRatio));
  this->addToolBar(Qt::LeftToolBarArea, ToolBar);

  plainTextEdit = new TextEditor;
  plainTextEdit->lineNumberArea->setForegroundRole(QPalette::HighlightedText);
  plainTextEdit->lineNumberArea->setBackgroundRole(QPalette::Base);
  this->setCentralWidget(plainTextEdit);

  connect(ui->actionNew, &QAction::triggered, plainTextEdit,
          &TextEditor::newDocument);
  connect(ui->actionOpen, &QAction::triggered, plainTextEdit,
          &TextEditor::open);
  connect(ui->actionSave, &QAction::triggered, plainTextEdit,
          &TextEditor::save);
  connect(ui->actionSave_As, &QAction::triggered, plainTextEdit,
          &TextEditor::saveAs);

  connect(ui->actionPreferences, &QAction::triggered, this,
          [&]() { PreferencesWindow(this, plainTextEdit).exec(); });

  connect(ui->actionPrint, &QAction::triggered, plainTextEdit,
          &TextEditor::print);
  connect(ui->actionUndo, &QAction::triggered, plainTextEdit,
          &TextEditor::undo);
  connect(ui->actionRedo, &QAction::triggered, plainTextEdit,
          &TextEditor::redo);
  connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quit);
  connect(ui->actionAbout, &QAction::triggered, this,
          [&]() { AboutWindow(this).exec(); });
  connect(plainTextEdit, &TextEditor::changeTitle, this,
          &MainWindow::changeWindowTitle);
  connect(plainTextEdit, &TextEditor::undoAvailable, this,
          [=](bool undoIsAvailable) {
            ui->actionUndo->setDisabled(!undoIsAvailable);
          });
  connect(plainTextEdit, &TextEditor::redoAvailable, this,
          [=](bool redoIsAvailable) {
            ui->actionRedo->setDisabled(!redoIsAvailable);
          });
  connect(plainTextEdit, &TextEditor::textChanged, this,
          &MainWindow::updateStatusBar);
  updateStatusBar();
  changeWindowTitle();
  ui->actionUndo->setDisabled(1);
  ui->actionRedo->setDisabled(1);

// Disable menu actions for unavailable features
#if !defined(QT_PRINTSUPPORT_LIB) || !QT_CONFIG(printer)
  ui->actionPrint->setEnabled(false);
#endif
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
          [&](bool isCopyAvailable) {
            ui->actionCopy->setDisabled(!isCopyAvailable);
          });
  connect(plainTextEdit, &TextEditor::copyAvailable, this,
          [&](bool isCutAvailable) {
            ui->actionCut->setDisabled(!isCutAvailable);
          });
  ui->actionCopy->setDisabled(1);
  ui->actionCut->setDisabled(1);
#endif
}

MainWindow::~MainWindow()
{
  delete ui;
  delete plainTextEdit;
}

void MainWindow::changeWindowTitle()
{
  if (!plainTextEdit->currentFile.isEmpty())
    setWindowTitle(plainTextEdit->currentFile.split("/").last() +
                   " - Notepanda");
  else
    setWindowTitle(tr("Untitled") + " - Notepanda");
}

void MainWindow::quit() { QCoreApplication::quit(); }

void MainWindow::updateStatusBar()
{
  statusBar()->showMessage(
      tr("Characters:") +
      QString::number(plainTextEdit->document()->characterCount() - 1) +
      " Lines:" + QString::number(plainTextEdit->document()->lineCount()));
}
