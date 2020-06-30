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
    settings->beginGroup("Editor");
    settings->setValue("FontFamily", QVariant(editorFontFamily));
    settings->setValue("FontSize", QVariant(editorFontSize));
    settings->setValue("ColorTheme", QVariant(editorColorTheme));
    settings->endGroup();
    settings->setValue("StyleTheme", QVariant(styleTheme));
}

/**
 * @brief Read general settings from `settings`.
 */
void ConfigManager::readGeneralSettings()
{
    if (settings->contains("Editor/FontFamily")) {
        editorFontFamily = settings->value("Editor/FontFamily").toString();
    } else {
        editorFontFamily =
            QFontDatabase::systemFont(QFontDatabase::FixedFont).toString();
    }
    styleTheme = settings->value("StyleTheme", "Fusion").toString();
    editorFontSize = settings->value("Editor/FontSize", 16).toInt();
    editorColorTheme =
        settings->value("Editor/ColorTheme", "Default").toString();
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

QString ConfigManager::getEditorColorTheme() const { return editorColorTheme; }
void ConfigManager::setEditorColorTheme(const QString &ctname)
{
    editorColorTheme = ctname;
};
