/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file preferenceswindow.cpp
 * @brief This file implements the PreferencesWindow class.
 *        All persistent application settings should be in PreferencesWindow.
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

        /*
         * Another implementation (better but harder) :
         for (const auto &theme : m_repository.themes()) addItem(...);
         * There is currently no plan to customize the theme of highlighting
         * So this implementation is temporarily used
        */

        ui->highlightThemeCombo->addItem("Breeze Dark");
        ui->highlightThemeCombo->addItem("Default");
        ui->highlightThemeCombo->addItem("Printing");
        ui->highlightThemeCombo->addItem("Solarized Dark");
        ui->highlightThemeCombo->addItem("Solarized Light");
    }
    ui->themeCombo->setCurrentText(configManager->getStyleTheme());
    ui->fontComboBox->setCurrentFont(
        QFont(configManager->getEditorFontFamily()));
    ui->spinBox->setValue(configManager->getEditorFontSize());
    ui->highlightThemeCombo->setCurrentText(
        configManager->getEditorColorTheme());
}
