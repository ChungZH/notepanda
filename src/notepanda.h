#ifndef NOTEPANDA_H
#define NOTEPANDA_H

#include <QMainWindow>
#include <QPlainTextEdit>

#include "texteditor.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class notepanda;
}
class TextEditor;
QT_END_NAMESPACE

class notepanda : public QMainWindow
{
  Q_OBJECT

 public:
  notepanda(QWidget *parent = nullptr);
  ~notepanda();

 private slots:
  void about();
  void changeWindowTitle();

 private:
  Ui::notepanda *ui;
  TextEditor *plainTextEdit;
};
#endif  // NOTEPANDA_H
