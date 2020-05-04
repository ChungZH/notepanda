#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QGridLayout>

#include <ui_aboutwindow.h>

class AboutWindow
      : public QDialog
      , private Ui::AboutWindow
{
  Q_OBJECT

 public:
  explicit AboutWindow(QWidget *parent = nullptr);

 private:
  Ui::AboutWindow *ui;
  QAction *a_AboutQt;
};

#endif // ABOUTWINDOW_H
