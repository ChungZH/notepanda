/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file configmanager.h
 * @brief This file declares the ConfigManager class for management
 *        configuration.
 */
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QFont>
#include <QObject>
#include <QSettings>
#include <QString>

/**
 * @brief The ConfigManager class
 */
class ConfigManager : public QObject
{
  Q_OBJECT
 public:
  explicit ConfigManager(const QString &configuration,
                         QObject *parent = nullptr);
  void save();
  void readGeneralSettings();
  QFont getEditorFontFamily() const;
  void setEditorFontFamily(const QString &fontname);
  QString getStyle() const;
  void setStyle(const QString &stylename);
  int getEditorFontSize() const;
  void setEditorFontSize(const int fontsize);

 private:
  QSettings *settings;
  QString configFile;
  QString editorFontFamily;
  QString style;
  int editorFontSize;
};

#endif  // CONFIGMANAGER_H
