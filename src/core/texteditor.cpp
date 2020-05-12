#include "texteditor.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QMessageBox>
#include <QPainter>
#include <QStyle>
#include <QTextBlock>
#include <QTextStream>

#include "../ui/linenumberarea.h"

TextEditor::TextEditor(QWidget *parent) : QPlainTextEdit(parent)
{
  lineNumberArea = new LineNumberArea(this);

  connect(this, &TextEditor::blockCountChanged, this,
          &TextEditor::updateLineNumberAreaWidth);
  connect(this, &TextEditor::updateRequest, this,
          &TextEditor::updateLineNumberArea);
  connect(this, &TextEditor::cursorPositionChanged, this,
          &TextEditor::highlightCurrentLine);

  updateLineNumberAreaWidth(0);
  highlightCurrentLine();

  cm = new ConfigManager(this);
  TextEditor::setFont(cm->getEditorFontFamily());
}

void TextEditor::newDocument()
{
  currentFile.clear();
  TextEditor::setPlainText(QString());
  emit changeTitle();
}

void TextEditor::open()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open the file"));
  QFile file(fileName);
  currentFile = fileName;
  if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Cannot open file: ") + file.errorString());
    return;
  }
  emit changeTitle();
  QTextStream in(&file);
  QString text = in.readAll();
  TextEditor::setPlainText(text);
  file.close();
}

void TextEditor::save()
{
  QString fileName;
  qDebug() << TextEditor::document()->characterCount();
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
    return;
  }

  emit changeTitle();
  QTextStream out(&file);
  QString text = TextEditor::toPlainText();
  out << text;
  file.close();
}

void TextEditor::saveAs()
{
  QString flieName = QFileDialog::getSaveFileName(this, tr("Save as"));
  QFile file(flieName);

  if (!file.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("Warning"),
                         tr("Cannot save file: ") + file.errorString());
    return;
  }

  currentFile = flieName;
  emit changeTitle();
  QTextStream out(&file);
  QString text = TextEditor::toPlainText();
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

    QColor lineColor = QColor(Qt::yellow).lighter(160);

    selection.format.setBackground(lineColor);
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
  // painter.fillRect(event->rect(), Qt::lightGray);

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
      painter.setPen(Qt::black);
      painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                       Qt::AlignRight, number);
    }

    block = block.next();
    top = bottom;
    bottom = top + qRound(blockBoundingRect(block).height());
    ++blockNumber;
  }
}
