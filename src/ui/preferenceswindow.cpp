#include "preferenceswindow.h"

#include <QObject>
#include <QStyleFactory>

#include "../core/texteditor.h"
#include "ui_preferenceswindow.h"

PreferencesWindow::PreferencesWindow(QWidget *parent, TextEditor *pTE)
    : QDialog(parent), ui(new Ui::PreferencesWindow), plainTextEdit(pTE)
{
  ui->setupUi(this);
  configManager = new ConfigManager;
  setWindowTitle(tr("Preferences - Notepanda"));

  ui->themeCombo->addItems(QStyleFactory::keys());
  ui->themeCombo->setCurrentText(configManager->getStyle());
  ui->fontComboBox->setCurrentFont(QFont(configManager->getEditorFontFamily()));

  connect(ui->themeCombo, &QComboBox::currentTextChanged, this,
          [&](const QString &curTheme) {
            QApplication::setStyle(QStyleFactory::create(curTheme));
            configManager->setStyle(curTheme);
          });
  connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, this,
          [this](QFont font) { plainTextEdit->setEditorFont(font); });
}
