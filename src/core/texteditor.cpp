/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file texteditor.cpp
 * @brief This file implements the TextEditor class.
 *
 */
#include "texteditor.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QMessageBox>
#include <QPainter>
#include <QStyle>
#include <QTextBlock>
#include <QTextStream>

// KSyntaxHighlighting
#include <definition.h>
#include <foldingregion.h>
#include <syntaxhighlighter.h>
#include <theme.h>

#include "../ui/linenumberarea.h"

TextEditor::TextEditor(ConfigManager *cfManager, QWidget *parent)
    : QPlainTextEdit(parent),
      m_highlighter(new KSyntaxHighlighting::SyntaxHighlighter(document())),
      configManager(cfManager)
{
  if (configManager->getColorTheme() == "Light")
    setTheme(m_repository.defaultTheme(
        (KSyntaxHighlighting::Repository::LightTheme)));
  else
    setTheme(
        m_repository.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme));

  // Line number area
  lineNumberArea = new LineNumberArea(this);

  connect(this, &TextEditor::blockCountChanged, this,
          &TextEditor::updateLineNumberAreaWidth);
  connect(this, &TextEditor::updateRequest, this,
          &TextEditor::updateLineNumberArea);
  connect(this, &TextEditor::cursorPositionChanged, this,
          &TextEditor::highlightCurrentLine);

  updateLineNumberAreaWidth(0);
  highlightCurrentLine();

  TextEditor::setFont(configManager->getEditorFontFamily());
  lineNumberArea->resize(0, 0);
}

bool TextEditor::maybeSave()
{
  if (!QPlainTextEdit::document()->isModified()) return true;
  const QMessageBox::StandardButton ret = QMessageBox::warning(
      this, tr("Application"),
      tr("The document has been modified.\n"
         "Do you want to save your changes?"),
      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  switch (ret) {
    case QMessageBox::Save:
      save();
      return 1;
    case QMessageBox::Cancel:
      return false;
    default:
      break;
  }
  return true;
}

void TextEditor::newDocument()
{
  if (maybeSave()) {
    currentFile.clear();
    TextEditor::setPlainText(QString());
    emit changeTitle();
  }
}

void TextEditor::open()
{
  if (maybeSave()) {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open the file"));
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
      QMessageBox::warning(this, tr("Warning"),
                           tr("Cannot open file: ") + file.errorString());
      qWarning() << "[WARN 1] Failed to open" << fileName << ":"
                 << file.errorString();
      return;
    }

    const auto def = m_repository.definitionForFileName(fileName);
    m_highlighter->setDefinition(def);

    emit changeTitle();
    QTextStream in(&file);
    QString text = in.readAll();
    setPlainText(text);
    file.close();
  }
}

/**
 * @brief Open from command line like `notepanda texteditor.cpp`.
 *        It doesn't need `maybeSave()` because at this time notepanda opens for
 * the first time.
 */
void TextEditor::openFile(const QString &fileName)
{
  QFile file(fileName);
  currentFile = fileName;
  if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Cannot open file: ") + file.errorString());
    qWarning() << "[WARN 2] Failed to open" << fileName << ":"
               << file.errorString();
    return;
  }

  const auto def = m_repository.definitionForFileName(fileName);
  m_highlighter->setDefinition(def);

  emit changeTitle();
  QTextStream in(&file);
  file.close();
  QString text = in.readAll();
  setPlainText(text);
}

void TextEditor::save()
{
  QString fileName;
  if (currentFile.isEmpty()) {
    fileName = QFileDialog::getSaveFileName(this, tr("Save"));
    currentFile = fileName;
  } else {
    fileName = currentFile;
  }
  QFile file(fileName);

  if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Cannot save file: ") + file.errorString());
    qWarning() << "[WARN 3] Failed to save" << fileName << ":"
               << file.errorString();
    return;
  }

  emit changeTitle();
  QTextStream out(&file);
  file.close();
  QString text = QPlainTextEdit::toPlainText();
  out << text;
}

void TextEditor::saveAs()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as"));
  QFile file(fileName);

  if (!file.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Cannot save file: ") + file.errorString());
    qWarning() << "[WARN 4] Failed to save" << fileName << ":"
               << file.errorString();
    return;
  }

  currentFile = fileName;
  emit changeTitle();
  QTextStream out(&file);
  QString text = QPlainTextEdit::toPlainText();
  out << text;
  file.close();
}

void TextEditor::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
  QPrinter printDev;
#if QT_CONFIG(printdialog)
  QPrintDialog dialog(&printDev, this);
  if (dialog.exec() == QDialog::Rejected) return;
#endif  // QT_CONFIG(printdialog)
QPlainTextEdit:
  print(&printDev);
#endif  // QT_CONFIG(printer)
}

void TextEditor::undo() { QPlainTextEdit::undo(); }

void TextEditor::redo() { QPlainTextEdit::redo(); }

void TextEditor::copy()
{
#if QT_CONFIG(clipboard)
  QPlainTextEdit::copy();
#endif
}

void TextEditor::paste()
{
#if QT_CONFIG(clipboard)
  QPlainTextEdit::paste();
#endif
}

void TextEditor::cut()
{
#if QT_CONFIG(clipboard)
  QPlainTextEdit::cut();
#endif
}

int TextEditor::lineNumberAreaWidth()
{
  int digits = 1;
  int max = qMax(1, blockCount());
  while (max >= 10) {
    max /= 10;
    ++digits;
  }

  int space = 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

  return space;
}

//![extraAreaWidth]

//![slotUpdateExtraAreaWidth]

void TextEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

//![slotUpdateExtraAreaWidth]

//![slotUpdateRequest]

void TextEditor::updateLineNumberArea(const QRect &rect, int dy)
{
  if (dy)
    lineNumberArea->scroll(0, dy);
  else
    lineNumberArea->update(0, rect.y(), lineNumberArea->sizeHint().width(),
                           rect.height());
  if (rect.contains(viewport()->rect())) updateLineNumberAreaWidth(0);
}

//![slotUpdateRequest]

//![resizeEvent]

void TextEditor::resizeEvent(QResizeEvent *e)
{
  QPlainTextEdit::resizeEvent(e);

  QRect cr = contentsRect();
  lineNumberArea->setGeometry(
      QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

//![resizeEvent]

//![cursorPositionChanged]

void TextEditor::highlightCurrentLine()
{
  QList<QTextEdit::ExtraSelection> extraSelections;

  if (!isReadOnly()) {
    QTextEdit::ExtraSelection selection;

    selection.format.setBackground(QColor(m_highlighter->theme().editorColor(
        KSyntaxHighlighting::Theme::CurrentLine)));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);
  }

  setExtraSelections(extraSelections);
}

//![cursorPositionChanged]

//![extraAreaPaintEvent_0]

void TextEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
  QPainter painter(lineNumberArea);
  QColor lineNumberAreaBackgroundColor;
  if (configManager->getColorTheme() == "Light") {
    // light
    lineNumberAreaBackgroundColor = Qt::lightGray;
    lineNumberAreaBackgroundColor.setAlphaF(0.75);
    m_lineNumbersColor = Qt::darkGray;
    m_lineNumbersColor.setAlphaF(0.9);
  } else {
    // dark
    lineNumberAreaBackgroundColor = KSyntaxHighlighting::Theme::BackgroundColor;
    lineNumberAreaBackgroundColor.setAlphaF(0.7);
    m_lineNumbersColor = Qt::lightGray;
    m_lineNumbersColor.setAlphaF(0.4);
  }

  painter.fillRect(event->rect(), lineNumberAreaBackgroundColor);

  //![extraAreaPaintEvent_0]

  //![extraAreaPaintEvent_1]
  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top =
      qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom = top + qRound(blockBoundingRect(block).height());
  //![extraAreaPaintEvent_1]

  //![extraAreaPaintEvent_2]
  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(m_lineNumbersColor);
      painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                       Qt::AlignCenter, number);
    }

    block = block.next();
    top = bottom;
    bottom = top + qRound(blockBoundingRect(block).height());
    ++blockNumber;
  }
}

void TextEditor::setTheme(const KSyntaxHighlighting::Theme &theme)
{
  auto pal = qApp->palette();
  if (theme.isValid()) {
    pal.setColor(
        QPalette::Base,
        theme.editorColor(KSyntaxHighlighting::Theme::BackgroundColor));
    pal.setColor(QPalette::Highlight,
                 theme.editorColor(KSyntaxHighlighting::Theme::TextSelection));
  }

  setPalette(pal);

  m_highlighter->setTheme(theme);
  m_highlighter->rehighlight();
  highlightCurrentLine();
}
void TextEditor::setEditorFont(const QFont &font)
{
  QPlainTextEdit::setFont(font);
  configManager->setEditorFontFamily(font.family());
}

void TextEditor::setEditorFontSize(const int &size)
{
  QFont font = configManager->getEditorFontFamily();
  font.setPointSize(size);
  QPlainTextEdit::setFont(font);
  configManager->setEditorFontSize(size);
  configManager->getEditorFontSize();
}

void TextEditor::setEditorColorTheme(const QString &ctname)
{
  if (ctname == "Light")
    setTheme(m_repository.defaultTheme(
        (KSyntaxHighlighting::Repository::LightTheme)));
  else
    setTheme(
        m_repository.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme));
}
