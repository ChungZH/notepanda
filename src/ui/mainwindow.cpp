/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file mainwindow.cpp
 * @brief This file implements the MainWindow class.
 *
 */
#include "mainwindow.h"

#include <QDebug>
#include <QSize>
#include <QStyle>
#include <QStyleFactory>
#include <QTextStream>
#include <QToolBar>

#include "aboutwindow.h"
#include "preferenceswindow.h"
#include "ui_mainwindow.h"
#include "ui_preferenceswindow.h"

MainWindow::MainWindow(ConfigManager *cfManager, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), configManager(cfManager)
{
  ui->setupUi(this);

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

  plainTextEdit = new TextEditor(configManager);
  this->setCentralWidget(plainTextEdit);

  connect(ui->actionNew, &QAction::triggered, plainTextEdit,
          &TextEditor::newDocument);
  connect(ui->actionOpen, &QAction::triggered, plainTextEdit,
          &TextEditor::open);
  connect(ui->actionSave, &QAction::triggered, plainTextEdit,
          &TextEditor::save);
  connect(ui->actionSave_As, &QAction::triggered, plainTextEdit,
          &TextEditor::saveAs);

  // PreferencesWindow START

  pfWindow = new PreferencesWindow(configManager, this);

  connect(ui->actionPreferences, &QAction::triggered,
          [&]() { pfWindow->exec(); });

  connect(pfWindow->ui->themeCombo, &QComboBox::currentTextChanged,
          [&](const QString &curTheme) {
            QApplication::setStyle(QStyleFactory::create(curTheme));
            configManager->setStyle(curTheme);
          });
  connect(pfWindow->ui->fontComboBox, &QFontComboBox::currentFontChanged,
          [&](const QFont font) { plainTextEdit->setEditorFont(font); });
  connect(pfWindow->ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
          [=](const int &value) { plainTextEdit->setEditorFontSize(value); });
  connect(pfWindow->ui->colorCombo, &QComboBox::currentTextChanged,
          [&](const QString &ctname) {
            plainTextEdit->setEditorColorTheme(ctname);
            configManager->setColorTheme(ctname);
            qDebug() << "D" << configManager->getStyle();
          });

  // User accepted, so change the `settings`.
  connect(pfWindow->ui->buttonBox, &QDialogButtonBox::accepted,
          [&]() { configManager->save(); });

  // User rejected, so read general settings, do not change the `settings` !!!
  // Otherwise it will not be restored
  connect(pfWindow->ui->buttonBox, &QDialogButtonBox::rejected, [&]() {
    // Restore the variables
    configManager->readGeneralSettings();

    // Restore TextEditor
    plainTextEdit->setEditorFont(configManager->getEditorFontFamily());
    plainTextEdit->setEditorFontSize(configManager->getEditorFontSize());
    plainTextEdit->setEditorColorTheme(configManager->getColorTheme());

    // Restore MainWindow
    QApplication::setStyle(QStyleFactory::create(configManager->getStyle()));

    // Restore PreferencesWindow
    pfWindow->resetAllValues(0);
  });

  // PW END

  connect(ui->actionPrint, &QAction::triggered, plainTextEdit,
          &TextEditor::print);
  connect(ui->actionUndo, &QAction::triggered, plainTextEdit,
          &TextEditor::undo);
  connect(ui->actionRedo, &QAction::triggered, plainTextEdit,
          &TextEditor::redo);
  connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quit);
  connect(ui->actionAbout, &QAction::triggered,
          [&]() { AboutWindow(this).exec(); });
  connect(plainTextEdit, &TextEditor::changeTitle, this,
          &MainWindow::changeWindowTitle);
  connect(plainTextEdit, &TextEditor::undoAvailable, [=](bool undoIsAvailable) {
    ui->actionUndo->setDisabled(!undoIsAvailable);
  });
  connect(plainTextEdit, &TextEditor::redoAvailable, [=](bool redoIsAvailable) {
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
  connect(plainTextEdit, &TextEditor::copyAvailable, [&](bool isCopyAvailable) {
    ui->actionCopy->setDisabled(!isCopyAvailable);
  });
  connect(plainTextEdit, &TextEditor::copyAvailable, [&](bool isCutAvailable) {
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
