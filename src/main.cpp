#include <QApplication>
#include <QStyleFactory>
#include <QDebug>

#include "./ui/mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication App(argc, argv);
  qDebug() << QStyleFactory::keys();
  QApplication::setStyle(QStyleFactory::create("Fusion"));
  MainWindow notepanda;
  notepanda.show();
  return App.exec();
}
