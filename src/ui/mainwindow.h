/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file mainwindow.h
 * @brief This file declares the MainWindow class.
 *
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QColorDialog>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QToolBar>

#include "../core/configmanager.h"
#include "../core/texteditor.h"
#include "preferenceswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
class TextEditor;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow(ConfigManager *confManager, QWidget *parent = nullptr);
  ~MainWindow();
  TextEditor *plainTextEdit;

 protected:
  void closeEvent(QCloseEvent *event) override;

 private slots:
  void changeWindowTitle();
  void quit();
  void updateStatusBar();
  void normalMode(bool first);
  void stickyNoteMode();
  void documentWasModified();

 private:
  Ui::MainWindow *ui;
  QToolBar *ToolBar;
  ConfigManager *configManager;
  PreferencesWindow *pfWindow;
  /**
   * @brief currentMode
   *    0: normal mode
   *    1: sticky mode
   */
  int currentMode;
  bool isPintotop;

  // Sticky note mode

  QToolBar *SToolBar;
  QAction *changeBgColor;
  QColorDialog *ColorDialog;
  QColor currentColor;

  //
};
#endif  // MAINWINDOW_H
