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

  resetAllValues(1);
}

void PreferencesWindow::resetAllValues(const bool isFirst)
{
  if (isFirst) {
    ui->themeCombo->addItems(QStyleFactory::keys());
    ui->colorCombo->addItem("Light");
    ui->colorCombo->addItem("Dark");
  }
  ui->themeCombo->setCurrentText(configManager->getStyle());
  ui->fontComboBox->setCurrentFont(QFont(configManager->getEditorFontFamily()));
  ui->spinBox->setValue(configManager->getEditorFontSize());
  ui->colorCombo->setCurrentText(configManager->getColorTheme());
}
