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
#include <QJsonDocument>

ConfigManager::ConfigManager(const QString &configuration, QObject *parent)
    : QObject(parent), configFile(configuration)
{
  const QSettings::Format JsonFormat =
      QSettings::registerFormat("json", readJsonFile, writeJsonFile);
  settings = new QSettings(configFile, JsonFormat, this);
  readGeneralSettings();
}

bool ConfigManager::readJsonFile(QIODevice &device, QSettings::SettingsMap &map)
{
  QJsonParseError error;
  map = QJsonDocument::fromJson(device.readAll(), &error).toVariant().toMap();
  return error.error == QJsonParseError::NoError;
}

bool ConfigManager::writeJsonFile(QIODevice &device,
                                  const QSettings::SettingsMap &map)
{
  const auto json = QJsonDocument::fromVariant(map).toJson();
  return device.write(json) == json.size();
}

/**
 * @brief Save the settings.
 */
void ConfigManager::save()
{
  settings->setValue("EditorFontFamily", QVariant(editorFontFamily));
  settings->setValue("StyleTheme", QVariant(styleTheme));
  settings->setValue("EditorFontSize", QVariant(editorFontSize));
  settings->setValue("ColorTheme", QVariant(colorTheme));
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
  styleTheme = settings->value("StyleTheme", "Fusion").toString();
  editorFontSize = settings->value("EditorFontSize", 16).toInt();
  colorTheme = settings->value("ColorTheme", "Default").toString();
}

QString ConfigManager::getEditorFontFamily() const { return editorFontFamily; }

void ConfigManager::setEditorFontFamily(const QString &fontname)
{
  editorFontFamily = fontname;
}

QString ConfigManager::getStyleTheme() const { return styleTheme; }

void ConfigManager::setStyleTheme(const QString &stylename)
{
  styleTheme = stylename;
}

int ConfigManager::getEditorFontSize() const { return editorFontSize; }

void ConfigManager::setEditorFontSize(const int &fontsize)
{
  editorFontSize = fontsize;
}

QString ConfigManager::getColorTheme() const { return colorTheme; }
void ConfigManager::setColorTheme(const QString &ctname)
{
  colorTheme = ctname;
};
