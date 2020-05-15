/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file aboutwindow.cpp
 * @brief This file implements the AboutWindow class.
 *
 */
#include "aboutwindow.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::AboutWindow)
{
  ui->setupUi(this);
  setWindowTitle("About - Notepanda");

  QFile ver(":/asset/makespec/VERSION");
  QFile verSuf(":/asset/makespec/VERSIONSUFFIX");
  QFile buildVer(":/asset/makespec/BUILDVERSION");
  ver.open(QFile::ReadOnly);
  verSuf.open(QFile::ReadOnly);
  buildVer.open(QFile::ReadOnly);

  QTextStream t_ver(&ver);
  QTextStream t_vSf(&verSuf);
  QTextStream t_bVe(&buildVer);

  QString verString =
      t_ver.readAll() + t_vSf.readAll() + " BV" + t_bVe.readAll();

  ui->label_5->setText(verString);
  a_AboutQt = new QAction(tr("About Qt"), this);
  ui->AboutQt->setDefaultAction(a_AboutQt);

  connect(a_AboutQt, &QAction::triggered, this, &QApplication::aboutQt);
}
