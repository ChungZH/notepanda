#include "configmanager.h"

#include <QDebug>
#include <QFontDatabase>

ConfigManager::ConfigManager(QObject *parent) : QObject(parent)
{
  settings = new QSettings;
  readGeneralSettings();
}

void ConfigManager::readGeneralSettings()
{
  if (settings->contains("EditorFontFamily")) {
    editorFontFamily = QFont(settings->value("EditorFontFamily").toString());
  } else {
    editorFontFamily = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  }
  style = settings->value("Style", "Fusion").toString();
}

QFont ConfigManager::getEditorFontFamily() const { return editorFontFamily; }

void ConfigManager::setEditorFontFamily(const QString &fontname)
{
  editorFontFamily = QFont(fontname);
}

QString ConfigManager::getStyle() const { return style; }

void ConfigManager::setStyle(const QString &stylename) { style = stylename; }
