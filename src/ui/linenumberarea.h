#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>

#include "../core/texteditor.h"

class LineNumberArea : public QWidget
{
  Q_OBJECT

 public:
  LineNumberArea(TextEditor *editor) : QWidget(editor), textEditor(editor) {}
  ~LineNumberArea(){}

  QSize sizeHint() const override
  {
    return QSize(textEditor->lineNumberAreaWidth(), 0);
  }

 protected:
  void paintEvent(QPaintEvent *event) override
  {
    textEditor->lineNumberAreaPaintEvent(event);
  }

 private:
  TextEditor *textEditor;
};

#endif  // LINENUMBERAREA_H
