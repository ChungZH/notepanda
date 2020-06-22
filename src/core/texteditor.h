/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file texteditor.h
 * @brief This file declares the TextEditor class.
 *
 */
#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <repository.h>

#include <QPlainTextEdit>

#include "configmanager.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QFileDialog;
QT_END_NAMESPACE

namespace KSyntaxHighlighting
{
class SyntaxHighlighter;
}

class TextEditor : public QPlainTextEdit
{
  Q_OBJECT

 public:
  TextEditor(ConfigManager *cfManager, QWidget *parent = nullptr);
  QString currentFile;
  void lineNumberAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();
  void openFile(const QString &fileName);

 protected:
  void resizeEvent(QResizeEvent *event) override;
  void contextMenuEvent(QContextMenuEvent *event) override;

 public slots:
  bool maybeSave();
  void newDocument();
  void open();
  void save();
  void saveAs();
  void print();
  void undo();
  void redo();
  void copy();
  void paste();
  void cut();
  void updateLineNumberAreaWidth(int newBlockCount);
  void highlightCurrentLine();
  void updateLineNumberArea(const QRect &rect, int dy);
  void setEditorFont(const QFont &font);
  void setEditorFontSize(const int &size);
  void setEditorColorTheme(const QString &ctname);
  void setCurrentFile(const QString &fileName);

  /**
   * @brief switchMode
   * @param mode
   *    0: normal mode
   *    1: sticky mode
   */
  void switchMode(const int &mode);

 private:
  ConfigManager *configManager;
  KSyntaxHighlighting::Repository m_repository;
  KSyntaxHighlighting::SyntaxHighlighter *m_highlighter;
  void setTheme(const KSyntaxHighlighting::Theme &theme);
  QWidget *lineNumberArea;
  int currentMode;

 signals:
  void changeTitle();
  void modifiedFalse();
};

#endif  // TEXTEDITOR_H
