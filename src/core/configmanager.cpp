#include "configmanager.h"

#include <QDebug>
#include <QFontDatabase>

ConfigManager::ConfigManager()
{
  editorFontFamily = QFontDatabase::systemFont(QFontDatabase::FixedFont);
}
