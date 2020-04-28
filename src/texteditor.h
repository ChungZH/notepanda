#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QFileDialog;
QT_END_NAMESPACE

class TextEditor : public QPlainTextEdit
{
  Q_OBJECT

 public:
  TextEditor(QWidget *parent = nullptr);
  QString currentFile;

 public slots:
  void newDocument();
  void open();
  void save();
  void saveAs();

  // move to public
  /*
 private:
  QString currentFile;*/

 signals:
  void changeTitle();
};

#endif  // TEXTEDITOR_H
