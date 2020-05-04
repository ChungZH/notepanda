#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include "ui_aboutwindow.h"

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QGridLayout>

QT_BEGIN_NAMESPACE
namespace Ui
{
class AboutWindow;
}
QT_END_NAMESPACE


class AboutWindow : public QDialog
{
  Q_OBJECT

 public:
  AboutWindow(QWidget *parent = nullptr);

 private:
  Ui::AboutWindow *ui;
  QAction *a_AboutQt;
};

#endif // ABOUTWINDOW_H
