#ifndef NOTEPANDA_H
#define NOTEPANDA_H

#include <QMainWindow>
#include <QPlainTextEdit>

#include "texteditor.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
class TextEditor;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void about();
  void changeWindowTitle();
  void setActUndoState(bool available);
  void setActRedoState(bool available);

 private:
  Ui::MainWindow *ui;
  TextEditor *plainTextEdit;
};
#endif  // NOTEPANDA_H
