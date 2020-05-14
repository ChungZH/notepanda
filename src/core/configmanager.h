#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QFont>
#include <QObject>
#include <QSettings>
#include <QString>

class ConfigManager : public QObject
{
  Q_OBJECT
 public:
  ConfigManager(QObject *parent = nullptr);
  void readGeneralSettings();
  QFont getEditorFontFamily() const;
  void setEditorFontFamily(const QString &fontname);
  QString getStyle() const;

 private:
  QSettings *settings;
  QString configFile;
  QFont editorFontFamily;
  QString style;
};

#endif  // CONFIGMANAGER_H
