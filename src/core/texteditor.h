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
  void setFont(const QFont &font);

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
