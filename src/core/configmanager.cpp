#include "configmanager.h"

#include <QDebug>
#include <QFontDatabase>

ConfigManager::ConfigManager(QObject *parent) : QObject(parent)
{
  settings = new QSettings;
  readGeneralSettings();
}

QFont ConfigManager::getEditorFontFamily() const { return editorFontFamily; }

void ConfigManager::readGeneralSettings()
{
  if (settings->contains("EditorFontFamily")) {
    editorFontFamily = QFont(settings->value("EditorFontFamily").toString());
  } else {
    editorFontFamily = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  }
}
