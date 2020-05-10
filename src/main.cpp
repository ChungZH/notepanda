#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QStyleFactory>

#include "./ui/mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication App(argc, argv);
  QApplication::setStyle(QStyleFactory::create("Fusion"));

  App.setOrganizationName("ChungZH");
  App.setApplicationName("Notepanda");
  App.setApplicationVersion("0.0.1-rc.1");

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addPositionalArgument("source", "The source file to open.");
  parser.process(App);

  MainWindow notepanda;
  notepanda.show();
  if (parser.positionalArguments().size() == 1)
    notepanda.plainTextEdit->openFile(parser.positionalArguments().at(0));
  return App.exec();
}
