#ifndef NOTEPANDA_H
#define NOTEPANDA_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QToolBar>

#include "../core/texteditor.h"

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
  void quit();
  void updateStatusBar();

 private:
  Ui::MainWindow *ui;
  TextEditor *plainTextEdit;
  QToolBar *ToolBar;
};
#endif  // NOTEPANDA_H
