/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file preferenceswindow.h
 * @brief This file declares the PreferencesWindow class.
 *
 */
#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include <QObject>
#include <QPlainTextEdit>
#include <QWidget>

#include "../core/configmanager.h"
#include "../core/texteditor.h"
#include "ui_preferenceswindow.h"

class PreferencesWindow : public QDialog, private Ui::PreferencesWindow
{
  Q_OBJECT
 public:
  PreferencesWindow(QWidget *parent = nullptr, TextEditor *pTE = nullptr);
  Ui::PreferencesWindow *ui;

 private:
  ConfigManager *configManager;
  TextEditor *plainTextEdit;
};

#endif  // PREFERENCESWINDOW_H
