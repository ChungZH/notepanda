#include "preferenceswindow.h"

#include <QStyleFactory>

#include "ui_preferenceswindow.h"

PreferencesWindow::PreferencesWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::PreferencesWindow)
{
  ui->setupUi(this);
  setWindowTitle(tr("Preferences - Notepanda"));

  ui->themeCombo->addItems(QStyleFactory::keys());
}
