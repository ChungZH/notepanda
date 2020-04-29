#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication EditorApp(argc, argv);
  MainWindow notepanda;
  notepanda.show();
  return EditorApp.exec();
}
