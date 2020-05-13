#include "preferenceswindow.h"

#include <QObject>
#include <QStyleFactory>

#include "../core/texteditor.h"
#include "ui_preferenceswindow.h"

PreferencesWindow::PreferencesWindow(QWidget *parent, QPlainTextEdit *pTE)
    : QDialog(parent), ui(new Ui::PreferencesWindow), plainTextEdit(pTE)
{
  ui->setupUi(this);
  configManager = new ConfigManager;
  setWindowTitle(tr("Preferences - Notepanda"));

  ui->themeCombo->addItems(QStyleFactory::keys());
  ui->fontComboBox->setCurrentFont(QFont(configManager->getEditorFontFamily()));

  connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, this,
          [this](QFont font) { plainTextEdit->setFont(font); });
}
