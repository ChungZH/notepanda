#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include <QObject>
#include <QPlainTextEdit>
#include <QWidget>

#include "../core/configmanager.h"
#include "../core/texteditor.h"
#include "ui_preferenceswindow.h"

class PreferencesWindow : public QDialog, private Ui::PreferencesWindow
{
  Q_OBJECT
 public:
  PreferencesWindow(QWidget *parent = nullptr, TextEditor *pTE = nullptr);
  Ui::PreferencesWindow *ui;

 private:
  ConfigManager *configManager;
  TextEditor *plainTextEdit;
};

#endif  // PREFERENCESWINDOW_H
