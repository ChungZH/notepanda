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
  TextEditor(QWidget *parent = nullptr);
  QString currentFile;
  void lineNumberAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();

  void openFile(const QString &fileName);

 protected:
  void resizeEvent(QResizeEvent *event) override;

 public slots:
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

 private:
  ConfigManager *configManager;

 private:
  void setTheme(const KSyntaxHighlighting::Theme &theme);
  KSyntaxHighlighting::Repository m_repository;
  KSyntaxHighlighting::SyntaxHighlighter *m_highlighter;
  QWidget *lineNumberArea;
  QColor m_lineNumbersColor;

 signals:
  void changeTitle();
};

#endif  // TEXTEDITOR_H
