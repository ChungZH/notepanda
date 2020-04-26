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
    connect(ui->actionSave, &QAction::triggered, this, &notepanda::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &notepanda::saveAs);
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

void notepanda::save()
{
    QString fileName;

    // 如果还没有打开文件
    if (currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, tr("Save"));
        currentFile = fileName;
    } else {
        fileName = currentFile;
    }
    QFile file(fileName);

    // 处理只读以及其他错误
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot save file: ") + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void notepanda::saveAs()
{
    QString flieName = QFileDialog::getSaveFileName(this, tr("Save as"));
    QFile file(flieName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot save file: ") + file.errorString());
               return;
    }

    currentFile = flieName;
    setWindowTitle(flieName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void notepanda::about()
{
   QMessageBox::about(this, tr("About Notepanda"),
                tr("<center><b>Notepanda</b></center> <br> <center>A simple cross-platform notepad. Based on Qt and C++.</center><br>"
                   "<b>Vesrion:</b> 0.0.1-alpha<br>"
                   "Copyright © 2020 ChungZH & Shawfing."));
}


