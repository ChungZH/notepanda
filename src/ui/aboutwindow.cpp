#include "aboutwindow.h"
#include "ui_aboutwindow.h"

#include <QApplication>
#include <QAction>

AboutWindow::AboutWindow(QWidget *parent)
      : QDialog(parent), ui(new Ui::AboutWindow)
{
  ui->setupUi(this);


  a_AboutQt = new QAction(tr("About Qt"), this);
  ui->AboutQt->setDefaultAction(a_AboutQt);

  connect(a_AboutQt, &QAction::triggered, this, &QApplication::aboutQt);

}
