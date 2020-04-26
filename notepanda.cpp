#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include "notepanda.h"
#include "ui_notepanda.h"

notepanda::notepanda(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::notepanda)
{
    ui->setupUi(this);
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);

    connect(ui->actionNew, &QAction::triggered, this, &notepanda::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &notepanda::open);
    connect(ui->actionAbout, &QAction::triggered, this, &notepanda::about);
}

notepanda::~notepanda()
{
    delete ui;
}

void notepanda::newDocument()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
}

void notepanda::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open the file"));
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot open file: ") + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void notepanda::about()
{
   QMessageBox::about(this, tr("About Notepanda"),
                tr("<b>Notepanda</b> - A simple cross-platform notepad. Based on Qt and C++.<br>"
                   "<b>Vesrion:</b> 0.0.1-alpha<br/>"
                   "Copyright © 2020 ChungZH & Shawfing."));
}


