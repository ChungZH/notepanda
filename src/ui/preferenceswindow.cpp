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

PreferencesWindow::PreferencesWindow(QWidget *parent, TextEditor *pTE)
    : QDialog(parent), ui(new Ui::PreferencesWindow), plainTextEdit(pTE)
{
  ui->setupUi(this);
  configManager = new ConfigManager;
  setWindowTitle(tr("Preferences - Notepanda"));

  ui->themeCombo->addItems(QStyleFactory::keys());
  ui->themeCombo->setCurrentText(configManager->getStyle());
  ui->fontComboBox->setCurrentFont(QFont(configManager->getEditorFontFamily()));
  ui->spinBox->setValue(configManager->getEditorFontSize());

  connect(ui->themeCombo, &QComboBox::currentTextChanged, this,
          [&](const QString &curTheme) {
            QApplication::setStyle(QStyleFactory::create(curTheme));
            configManager->setStyle(curTheme);
          });
  connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, this,
          [this](const QFont font) { plainTextEdit->setEditorFont(font); });
  connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
          [=](const int &value) {
            plainTextEdit->setEditorFontSize(value);
            qDebug() << "in PreferencesWindow"
                     << configManager->getEditorFontSize();
          });
  connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
          [&]() { configManager->save(); });
}
