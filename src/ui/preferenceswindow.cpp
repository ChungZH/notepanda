/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file preferenceswindow.cpp
 * @brief This file implements the PreferencesWindow class.
 *
 */
#include "preferenceswindow.h"

#include <QDebug>
#include <QObject>
#include <QSpinBox>
#include <QStyleFactory>

#include "../core/texteditor.h"
#include "ui_preferenceswindow.h"

PreferencesWindow::PreferencesWindow(ConfigManager *cfManager, QWidget *parent)
    : QDialog(parent), ui(new Ui::PreferencesWindow), configManager(cfManager)
{
  ui->setupUi(this);
  setWindowTitle(tr("Preferences - Notepanda"));

  resetAllValues();
}

void PreferencesWindow::resetAllValues()
{
  ui->themeCombo->addItems(QStyleFactory::keys());
  ui->themeCombo->setCurrentText(configManager->getStyle());
  ui->fontComboBox->setCurrentFont(QFont(configManager->getEditorFontFamily()));
  ui->spinBox->setValue(configManager->getEditorFontSize());
}
