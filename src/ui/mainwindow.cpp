/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file mainwindow.cpp
 * @brief This file implements the MainWindow class.
 *        It is the main window of notepanda.
 *
 */

#include "mainwindow.h"

#include <QDebug>
#include <QPainter>
#include <QSize>
#include <QStyle>
#include <QStyleFactory>
#include <QTextStream>
#include <QToolBar>

#include "aboutwindow.h"
#include "preferenceswindow.h"
#include "ui_preferenceswindow.h"

MainWindow::MainWindow(ConfigManager *cfManager, QWidget *parent)
    : QMainWindow(parent), configManager(cfManager)
{
    resize(800, 600);
    setupUi();
    setBaseSize(size());

    ToolBar = new QToolBar;

    isPintotop = 0;

    // Sticky note mode

    SToolBar = new QToolBar;
    changeBgColor = new QAction;
    changeBgColor->setToolTip(tr("Change background color"));
    changeBgColor->setIconText("BG Color");
    currentColor = "#AAFFFF";

    SToolBar->addAction(changeBgColor);
    SToolBar->addAction(actionNormalmode);
    SToolBar->addAction(actionPin_to_top);

    addToolBar(Qt::ToolBarArea::BottomToolBarArea, SToolBar);
    SToolBar->setVisible(0);

    ColorDialog = new QColorDialog;

    connect(changeBgColor, &QAction::triggered, [&]() { ColorDialog->open(); });
    connect(ColorDialog, &QColorDialog::currentColorChanged,
            [&](const QColor &color) {
                currentColor = color;
                plainTextEdit->setStyleSheet("background-color: " +
                                             currentColor.name());
            });

    //

    this->addToolBar(Qt::LeftToolBarArea, ToolBar);

    normalMode(1);

    plainTextEdit = new TextEditor(configManager);
    this->setCentralWidget(plainTextEdit);

    connect(actionNew, &QAction::triggered, plainTextEdit,
            &TextEditor::newDocument);
    connect(actionOpen, &QAction::triggered, plainTextEdit, &TextEditor::open);
    connect(actionSave, &QAction::triggered, plainTextEdit, &TextEditor::save);
    connect(actionSave_As, &QAction::triggered, plainTextEdit,
            &TextEditor::saveAs);
    connect(actionPrint, &QAction::triggered, plainTextEdit,
            &TextEditor::print);
    connect(actionRedo, &QAction::triggered, plainTextEdit, &TextEditor::redo);
    connect(actionUndo, &QAction::triggered, plainTextEdit, &TextEditor::undo);
    connect(actionReadOnlyMode, &QAction::triggered, [&]() {
        plainTextEdit->setReadOnly(!plainTextEdit->isReadOnly());
        updateStatusBar();
    });
    connect(plainTextEdit, &TextEditor::readOnlyChanged, this,
            &MainWindow::updateStatusBar);

    previewPanel = new QTextBrowser(this);
    DockWidget = new QDockWidget(tr("Preview panel"), this);
    DockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
                                Qt::RightDockWidgetArea);
    DockWidget->setWidget(previewPanel);
    addDockWidget(Qt::RightDockWidgetArea, DockWidget);
    DockWidget->setVisible(0);

    // PreferencesWindow START

    pfWindow = new PreferencesWindow(configManager, this);

    connect(actionPreferences, &QAction::triggered,
            [&]() { pfWindow->exec(); });

    connect(pfWindow->ui->themeCombo, &QComboBox::currentTextChanged,
            [&](const QString &curTheme) {
                QApplication::setStyle(QStyleFactory::create(curTheme));
                configManager->setStyleTheme(curTheme);
            });
    connect(pfWindow->ui->fontComboBox, &QFontComboBox::currentFontChanged,
            [&](const QFont font) { plainTextEdit->setEditorFont(font); });
    connect(pfWindow->ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](const int &value) { plainTextEdit->setEditorFontSize(value); });
    connect(pfWindow->ui->highlightThemeCombo, &QComboBox::currentTextChanged,
            [&](const QString &ctname) {
                plainTextEdit->setEditorColorTheme(ctname);
                configManager->setEditorColorTheme(ctname);
            });

    // User accepted, so change the `settings`.
    connect(pfWindow->ui->buttonBox, &QDialogButtonBox::accepted,
            [&]() { configManager->save(); });

    // User rejected, so read general settings, do not change the `settings` !!!
    // Otherwise it will not be restored
    connect(pfWindow->ui->buttonBox, &QDialogButtonBox::rejected, [&]() {
        // Restore the variables
        configManager->readGeneralSettings();

        // Restore TextEditor
        plainTextEdit->setEditorFont(configManager->getEditorFontFamily());
        plainTextEdit->setEditorFontSize(configManager->getEditorFontSize());
        plainTextEdit->setEditorColorTheme(
            configManager->getEditorColorTheme());

        // Restore MainWindow
        QApplication::setStyle(
            QStyleFactory::create(configManager->getStyleTheme()));

        // Restore PreferencesWindow
        pfWindow->resetAllValues(0);
    });

    // PW END

    connect(plainTextEdit, &TextEditor::changeTitle, this,
            &MainWindow::changeWindowTitle);
    connect(plainTextEdit->document(), &QTextDocument::contentsChanged, this,
            &MainWindow::documentWasModified);
    connect(plainTextEdit, &TextEditor::modifiedFalse,
            [=]() { setWindowModified(false); });
    connect(plainTextEdit, &TextEditor::undoAvailable,
            [=](bool undoIsAvailable) {
                actionUndo->setDisabled(!undoIsAvailable);
            });
    connect(plainTextEdit, &TextEditor::redoAvailable,
            [=](bool redoIsAvailable) {
                actionRedo->setDisabled(!redoIsAvailable);
            });

    connect(plainTextEdit, &TextEditor::textChanged, this,
            &MainWindow::updateStatusBar);

    updateStatusBar();
    changeWindowTitle();
    actionUndo->setDisabled(1);
    actionRedo->setDisabled(1);

// Disable menu actions for unavailable features
#if !defined(QT_PRINTSUPPORT_LIB) || !QT_CONFIG(printer)
    actionPrint->setEnabled(false);
#endif
#if !QT_CONFIG(clipboard)
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);
#else
    connect(actionCopy, &QAction::triggered, plainTextEdit, &TextEditor::copy);
    connect(actionPaste, &QAction::triggered, plainTextEdit,
            &TextEditor::paste);
    connect(actionCut, &QAction::triggered, plainTextEdit, &TextEditor::cut);
    connect(plainTextEdit, &TextEditor::copyAvailable,
            [&](bool isCopyAvailable) {
                actionCopy->setDisabled(!isCopyAvailable);
            });
    connect(
        plainTextEdit, &TextEditor::copyAvailable,
        [&](bool isCutAvailable) { actionCut->setDisabled(!isCutAvailable); });
    actionCopy->setDisabled(1);
    actionCut->setDisabled(1);
#endif
}

MainWindow::~MainWindow() { delete plainTextEdit; }

void MainWindow::setupUi()
{
    actionNew =
        new QAction(QIcon(":/remixicons/images/remixicons/file-add-line.svg"),
                    tr("&New"), this);
    actionNew->setShortcut(QKeySequence::New);

    actionOpen =
        new QAction(QIcon(":/remixicons/images/remixicons/file-3-line.svg"),
                    tr("&Open"), this);
    actionOpen->setShortcut(QKeySequence::Open);

    actionSave =
        new QAction(QIcon(":/remixicons/images/remixicons/save-line.svg"),
                    tr("&Save"), this);
    actionSave->setShortcut(QKeySequence::Save);

    actionSave_As =
        new QAction(QIcon(":/remixicons/images/remixicons/save-2-line.svg"),
                    tr("&Save as"), this);
    actionSave_As->setShortcut(QKeySequence::SaveAs);

    actionUndo = new QAction(
        QIcon(":/remixicons/images/remixicons/arrow-go-back-line.svg"),
        tr("&Undo"), this);
    actionUndo->setShortcut(QKeySequence::Undo);

    actionRedo = new QAction(
        QIcon(":/remixicons/images/remixicons/arrow-go-forward-line.svg"),
        tr("&Redo"), this);
    actionRedo->setShortcut(QKeySequence::Redo);

    actionCopy =
        new QAction(QIcon(":/remixicons/images/remixicons/clipboard-line.svg"),
                    tr("&Copy"), this);
    actionCopy->setShortcut(QKeySequence::Copy);

    actionPaste =
        new QAction(QIcon(":/remixicons/images/remixicons/file-copy-line.svg"),
                    tr("&Paste"), this);
    actionPaste->setShortcut(QKeySequence::Paste);

    actionQuit =
        new QAction(QIcon(":/remixicons/images/remixicons/close-line.svg"),
                    tr("&Quit"), this);
    actionQuit->setShortcut(QKeySequence::Quit);
    connect(actionQuit, &QAction::triggered, this, &QCoreApplication::quit);

    actionCut = new QAction(
        QIcon(":/remixicons/images/remixicons/scissors-cut-line.svg"),
        tr("Cut"), this);
    actionCut->setShortcut(QKeySequence::Cut);

    actionPrint =
        new QAction(QIcon(":/remixicons/images/remixicons/printer-line.svg"),
                    tr("Print"), this);
    actionPrint->setShortcut(QKeySequence::Print);

    actionPreferences =
        new QAction(QIcon(":/remixicons/images/remixicons/settings-4-line.svg"),
                    tr("Preferences"), this);
    actionPreferences->setShortcut(QKeySequence::Preferences);

    actionSticky_note_mode = new QAction(
        QIcon(":/remixicons/images/remixicons/sticky-note-line.svg"),
        tr("Sticky note mode"), this);
    connect(actionSticky_note_mode, &QAction::triggered, [&]() {
        stickyNoteMode();
        actionSticky_note_mode->setDisabled(1);
        actionNormalmode->setEnabled(1);
    });

    actionNormalmode =
        new QAction(QIcon(":/remixicons/images/remixicons/file-edit-line.svg"),
                    tr("Normal mode"), this);
    connect(actionNormalmode, &QAction::triggered, [&]() {
        normalMode(0);
        actionNormalmode->setDisabled(1);
        actionSticky_note_mode->setEnabled(1);
    });

    actionPin_to_top =
        new QAction(QIcon(":/remixicons/images/remixicons/pushpin-line.svg"),
                    tr("Pin to top"), this);
    connect(actionPin_to_top, &QAction::triggered, [&]() {
        Qt::WindowFlags flags = this->windowFlags();
        if (!isPintotop) {
            this->setWindowFlags(flags | Qt::CustomizeWindowHint |
                                 Qt::WindowStaysOnTopHint);
            this->show();
        } else {
            this->setWindowFlags(
                flags ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
            this->show();
        }
        isPintotop = !isPintotop;
    });

    actionPreview_panel =
        new QAction(QIcon(":/remixicons/images/remixicons/eye-line.svg"),
                    tr("Preview panel"), this);
    connect(actionPreview_panel, &QAction::triggered,
            [&]() { DockWidget->setVisible(!DockWidget->isVisible()); });

    actionAbout = new QAction(
        QIcon(":/remixicons/images/remixicons/information-line.svg"),
        tr("&About"), this);
    connect(actionAbout, &QAction::triggered,
            [&]() { AboutWindow(this).exec(); });

    actionAboutQt = new QAction(tr("&About Qt"), this);
    connect(actionAboutQt, &QAction::triggered, this, &QApplication::aboutQt);

    actionReadOnlyMode = new QAction(tr("Read-Only mode"), this);

    menuFile = menuBar()->addMenu(tr("&File"));
    menuFile->addAction(actionNew);
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSave_As);
    menuFile->addSeparator();
    menuFile->addAction(actionPreferences);
    menuFile->addSeparator();
    menuFile->addAction(actionPrint);
    menuFile->addSeparator();
    menuFile->addAction(actionQuit);

    menuEdit = menuBar()->addMenu(tr("&Edit"));
    menuEdit->addAction(actionUndo);
    menuEdit->addAction(actionRedo);
    menuEdit->addSeparator();
    menuEdit->addAction(actionCopy);
    menuEdit->addAction(actionPaste);
    menuEdit->addAction(actionCut);
    menuEdit->addSeparator();
    menuEdit->addAction(actionReadOnlyMode);

    menuView = menuBar()->addMenu(tr("&View"));
    menuView->addAction(actionNormalmode);
    menuView->addAction(actionSticky_note_mode);
    menuView->addSeparator();
    menuView->addAction(actionPin_to_top);
    menuView->addSeparator();
    menuView->addAction(actionPreview_panel);

    menuAbout = menuBar()->addMenu(tr("&About"));
    menuAbout->addAction(actionAbout);
    menuAbout->addAction(actionAboutQt);

    menuBar()->addMenu(menuFile);
    menuBar()->addMenu(menuEdit);
    menuBar()->addMenu(menuView);
    menuBar()->addMenu(menuAbout);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (plainTextEdit->maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::changeWindowTitle()
{
    if (!plainTextEdit->currentFile.isEmpty())
        setWindowTitle(plainTextEdit->currentFile.split("/").last() +
                       "[*] - Notepanda");
    else
        setWindowTitle(tr("Untitled") + "[*] - Notepanda");
}

void MainWindow::updateStatusBar()
{
    if (currentMode != 1) {
        QString flags;
        if (plainTextEdit->isReadOnly() == 1) {
            flags += "[Read-Only]";
        }

        statusBar()->showMessage(
            tr("Characters:") +
            QString::number(plainTextEdit->document()->characterCount() - 1) +
            tr(" Lines:") +
            QString::number(plainTextEdit->document()->lineCount()) + flags);
    }
}

void MainWindow::normalMode(bool isFirst)
{
    if (!isFirst) {
        resize(baseSize());
        plainTextEdit->switchMode(0);
        ToolBar->setVisible(1);
        SToolBar->setVisible(0);
    } else {
        ToolBar->addAction(actionNew);
        ToolBar->addAction(actionOpen);
        ToolBar->addAction(actionSave);
        ToolBar->addAction(actionSave_As);
        ToolBar->addAction(actionPreferences);
        ToolBar->addAction(actionUndo);
        ToolBar->addAction(actionRedo);
        ToolBar->addAction(actionQuit);
        ToolBar->addAction(actionAbout);
        ToolBar->addAction(actionSticky_note_mode);
        ToolBar->addAction(actionPreview_panel);
    }
    actionPreferences->setEnabled(1);
    currentMode = 0;
}

/**
 * @brief Sticky Note Mode, like Microsoft Sticky Notes. For more details, see
 * <https://www.microsoft.com/en-us/p/microsoft-sticky-notes/9nblggh4qghw?activetab=pivot:overviewtab#>
 */
void MainWindow::stickyNoteMode()
{
    resize(baseSize() * 0.7);
    plainTextEdit->switchMode(1);
    plainTextEdit->setStyleSheet("background: " + currentColor.name());
    statusBar()->clearMessage();
    actionPreferences->setDisabled(1);

    ToolBar->setVisible(0);
    SToolBar->setVisible(1);

    currentMode = 1;
}

void MainWindow::documentWasModified()
{
    setWindowModified(plainTextEdit->document()->isModified());

    previewPanel->reload();
    previewPanel->setSource(plainTextEdit->currentFile);
}
