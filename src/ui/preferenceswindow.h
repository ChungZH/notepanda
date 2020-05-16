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

class PreferencesWindow : public QDialog, public Ui::PreferencesWindow
{
  Q_OBJECT
 public:
  PreferencesWindow(ConfigManager *cfManager, QWidget *parent = nullptr);
  void resetAllValues();
  Ui::PreferencesWindow *ui;

 private:
  ConfigManager *configManager;
};

#endif  // PREFERENCESWINDOW_H
