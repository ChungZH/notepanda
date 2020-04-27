
#include <QTextStream>
#include <QMessageBox>
#include "texteditor.h"
#include "notepanda.h"
#include "ui_notepanda.h"

notepanda::notepanda(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::notepanda)
{
    ui->setupUi(this);
    ui->setupUi(this);
    plainTextEdit = new TextEditor;
    this->setCentralWidget(plainTextEdit);

    connect(ui->actionNew, &QAction::triggered, plainTextEdit, &TextEditor::newDocument);
    connect(ui->actionOpen, &QAction::triggered, plainTextEdit, &TextEditor::open);
    connect(ui->actionSave, &QAction::triggered, plainTextEdit, &TextEditor::save);
    connect(ui->actionSave_As, &QAction::triggered, plainTextEdit, &TextEditor::saveAs);
    connect(ui->actionAbout, &QAction::triggered, this, &notepanda::about);
}

notepanda::~notepanda()
{
    delete ui;
    delete plainTextEdit;
}

void notepanda::about()
{
   QMessageBox::about(this, tr("About Notepanda"),
                tr("<center><b>Notepanda</b></center> <br> <center>A simple cross-platform notepad. Based on Qt and C++.</center><br>"
                   "<b>Vesrion:</b> 0.0.1-alpha<br>"
                   "Copyright © 2020 ChungZH & Shawfing."));
}


