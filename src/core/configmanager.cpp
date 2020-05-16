/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file configmanager.cpp
 * @brief This file implements the ConfigManager class.
 *
 */
#include "configmanager.h"

#include <QDebug>
#include <QFontDatabase>

ConfigManager::ConfigManager(const QString &configuration, QObject *parent)
    : QObject(parent), configFile(configuration)
{
  settings = new QSettings(configFile, QSettings::IniFormat, this);
  readGeneralSettings();
}

/**
 * @brief Save the settings.
 */
void ConfigManager::save()
{
  settings->setValue("EditorFontFamily", QVariant(editorFontFamily));
  settings->setValue("Style", QVariant(style));
  settings->setValue("EditorFontSize", QVariant(editorFontSize));
}

/**
 * @brief Read general settings from `settings`.
 */
void ConfigManager::readGeneralSettings()
{
  if (settings->contains("EditorFontFamily")) {
    editorFontFamily = settings->value("EditorFontFamily").toString();
  } else {
    editorFontFamily =
        QFontDatabase::systemFont(QFontDatabase::FixedFont).toString();
  }
  style = settings->value("Style", "Fusion").toString();
  editorFontSize = settings->value("EditorFontSize", 16).toInt();
}

QFont ConfigManager::getEditorFontFamily() const { return editorFontFamily; }

void ConfigManager::setEditorFontFamily(const QString &fontname)
{
  editorFontFamily = fontname;
}

QString ConfigManager::getStyle() const { return style; }

void ConfigManager::setStyle(const QString &stylename) { style = stylename; }

int ConfigManager::getEditorFontSize() const { return editorFontSize; }

void ConfigManager::setEditorFontSize(const int fontsize)
{
  editorFontSize = fontsize;
}
