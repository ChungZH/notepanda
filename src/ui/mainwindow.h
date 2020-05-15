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
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  TextEditor *plainTextEdit;

 private slots:
  void changeWindowTitle();
  void quit();
  void updateStatusBar();

 private:
  Ui::MainWindow *ui;
  QToolBar *ToolBar;
  ConfigManager *configManager;
};
#endif  // MAINWINDOW_H
