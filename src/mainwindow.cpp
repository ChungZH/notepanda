
#include "mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QTextStream>

#include "texteditor.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->setupUi(this);
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
  connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
  connect(plainTextEdit, &TextEditor::changeTitle, this,
          &MainWindow::changeWindowTitle);
  connect(plainTextEdit, &TextEditor::undoAvailable, this,
          &MainWindow::setActUndoState);
  connect(plainTextEdit, &TextEditor::redoAvailable, this,
          &MainWindow::setActRedoState);
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
  setWindowTitle(plainTextEdit->currentFile.split("/").last() + " - Notepanda");
}

void MainWindow::setActUndoState(bool available)
{
  // BE CAREFUL
  // !available
  // BUT NOT
  // available
  ui->actionUndo->setDisabled(!available);
}

void MainWindow::setActRedoState(bool available)
{
  ui->actionRedo->setDisabled(!available);
}
