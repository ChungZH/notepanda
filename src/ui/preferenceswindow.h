#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include <QObject>
#include <QPlainTextEdit>
#include <QWidget>

#include "../core/configmanager.h"
#include "ui_preferenceswindow.h"

class PreferencesWindow : public QDialog, private Ui::PreferencesWindow
{
  Q_OBJECT
 public:
  PreferencesWindow(QWidget *parent = nullptr, QPlainTextEdit *pTE = nullptr);
  Ui::PreferencesWindow *ui;

 private:
  ConfigManager *configManager;
  QPlainTextEdit *plainTextEdit;
};

#endif  // PREFERENCESWINDOW_H
