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
#include <QSizePolicy>
#include <QStyle>
#include <QStyleFactory>
#include <QTabBar>
#include <QTextStream>
#include <QToolBar>

#include "aboutwindow.h"
#include "preferenceswindow.h"
#include "ui_preferenceswindow.h"

MainWindow::MainWindow(ConfigManager *cfManager, QWidget *parent)
    : QMainWindow(parent),
      TabBar(new QTabBar),
      SToolBar(new QToolBar),
      ColorDialog(new QColorDialog),
      previewPanel(new QTextBrowser),
      ToolBar(new QToolBar),
      configManager(cfManager),
      curTabIndex(0),
      prevTabIndex(0),
      lastTabRemoveFlag(0),
      doNotSaveDataFlag(0)
{
    resize(800, 600);
    setupUi();
    setBaseSize(size());

    ToolBar->setIconSize(QSize(26, 26));

    isPintotop = 0;

    // Sticky note mode
    changeBgColor = new QAction(QIcon(":/icons/color_background.svg"),
                                tr("Change background color"), this);
    changeBgColor->setToolTip(tr("Change background color"));
    currentColor = "#AAFFFF";

    SToolBar->addAction(changeBgColor);
    SToolBar->addAction(actionNormalmode);
    SToolBar->addAction(actionPin_to_top);

    addToolBar(Qt::ToolBarArea::BottomToolBarArea, SToolBar);
    SToolBar->setVisible(0);

    connect(changeBgColor, &QAction::triggered, [&]() { ColorDialog->open(); });
    connect(ColorDialog, &QColorDialog::currentColorChanged,
            [&](const QColor &color) {
                currentColor = color;
                plainTextEdit->setStyleSheet("background-color: " +
                                             currentColor.name());
            });

    // Sticky note mode END

    this->addToolBar(Qt::LeftToolBarArea, ToolBar);

    normalMode(1);

    /** TODO: TabBar->setMovable(true);
        If move, index will change. So I must adjust `tabData`.
    */

    TabBar->setTabsClosable(true);
    TabToolBar = new QToolBar;
    TabToolBar->addWidget(TabBar);
    TabToolBar->setMovable(false);
    TabToolBar->setFloatable(false);
    this->addToolBar(Qt::TopToolBarArea, TabToolBar);

    plainTextEdit = new TextEditor(configManager);
    this->setCentralWidget(plainTextEdit);

    TabBar->addTab("Untitled.txt");
    tabData.push_back(TabData{"", "", 0});

    connect(TabBar, &QTabBar::currentChanged, [&](const int index) {
        prevTabIndex = curTabIndex;
        curTabIndex = index;

        if (curTabIndex == -1) curTabIndex = 0;
        if (prevTabIndex == -1) prevTabIndex = 0;

        if (TabBar->count() == 0) {
            prevTabIndex = 0;
            tabData.push_back(TabData{"", "", 0});
            TabBar->addTab("Untitled.txt");
        }

        if (lastTabRemoveFlag == true) {
            prevTabIndex = curTabIndex = index;
            lastTabRemoveFlag = false;
        } else if (doNotSaveDataFlag == true) {
            doNotSaveDataFlag = false;
        } else {
            saveTabData(prevTabIndex);
        }

        plainTextEdit->clear();
        if (index != -1) {
            plainTextEdit->setCurrentFile(tabData[index].fileName);
            plainTextEdit->setPlainText(tabData[index].plainText);
        }

        plainTextEdit->updateSyntaxHighlight();
        plainTextEdit->document()->setModified(tabData[index].isModified);
        changeWindowTitle();
    });

    connect(TabBar, &QTabBar::tabCloseRequested, [&](int index) {
        tabData.remove(index);
        lastTabRemoveFlag = true;
        TabBar->removeTab(index);

        if (TabBar->count() != 0) {
            TabData tempTD = tabData[TabBar->currentIndex()];
            plainTextEdit->setPlainText(tempTD.plainText);
            plainTextEdit->setCurrentFile(tempTD.fileName);
            plainTextEdit->document()->setModified(tempTD.isModified);
        }
        changeWindowTitle();
    });

    connect(actionNew, &QAction::triggered, [&]() {
        saveTabData(TabBar->currentIndex());

        if (plainTextEdit->newDocument()) {
            doNotSaveDataFlag = true;
            const int newIndex = TabBar->addTab("Untitled.txt");
            tabData.push_back(TabData{"", "", 0});
            TabBar->setCurrentIndex(newIndex);
        }
    });
    connect(actionOpen, &QAction::triggered, [&]() {
        saveTabData(TabBar->currentIndex());
        if (plainTextEdit->open()) {
            doNotSaveDataFlag = true;
            const int newIndex = TabBar->addTab(plainTextEdit->currentFileName);
            tabData.push_back(TabData{plainTextEdit->toPlainText(),
                                      plainTextEdit->currentFileName, 0});
            TabBar->setCurrentIndex(newIndex);
        }
    });
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
    /**
    if user change Read-Only mode in right-click menu of TextEditor
    than setChecked in MainWindow::actionReadOnlyMode
    */
    connect(plainTextEdit, &TextEditor::readOnlyChanged, [&]() {
        actionReadOnlyMode->setChecked(plainTextEdit->isReadOnly());
        updateStatusBar();
    });

    DockWidget = new QDockWidget(tr("Preview panel"), this);
    DockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
                                Qt::RightDockWidgetArea);
    DockWidget->setWidget(previewPanel);
    addDockWidget(Qt::RightDockWidgetArea, DockWidget);
    DockWidget->setVisible(0);

    previewPanel->setEnabled(0);

    connect(DockWidget, &QDockWidget::visibilityChanged, [&](bool visible) {
        actionPreview_panel->setChecked(visible);
        previewPanel->setEnabled(visible);
    });

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
    connect(
        plainTextEdit, &TextEditor::openFileInNewTab, [&](const QString &file) {
            saveTabData(TabBar->currentIndex());

            const int newIndex = TabBar->addTab(plainTextEdit->currentFileName);
            plainTextEdit->openFile(file);
            tabData.push_back(TabData{plainTextEdit->toPlainText(),
                                      plainTextEdit->currentFileName, 0});
            doNotSaveDataFlag = true;
            TabBar->setCurrentIndex(newIndex);
            changeWindowTitle();
        });

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
    actionNew = new QAction(QIcon(":/icons/add.svg"), tr("&New"), this);
    actionNew->setShortcut(QKeySequence::New);

    actionOpen = new QAction(QIcon(":/icons/open.svg"), tr("&Open"), this);
    actionOpen->setShortcut(QKeySequence::Open);

    actionSave = new QAction(QIcon(":/icons/save.svg"), tr("&Save"), this);
    actionSave->setShortcut(QKeySequence::Save);

    actionSave_As =
        new QAction(QIcon(":/icons/save_as.svg"), tr("&Save as"), this);
    actionSave_As->setShortcut(QKeySequence::SaveAs);

    actionUndo = new QAction(QIcon(":/icons/undo.svg"), tr("&Undo"), this);
    actionUndo->setShortcut(QKeySequence::Undo);

    actionRedo = new QAction(QIcon(":/icons/redo.svg"), tr("&Redo"), this);
    actionRedo->setShortcut(QKeySequence::Redo);

    actionCopy = new QAction(QIcon(":/icons/copy.svg"), tr("&Copy"), this);
    actionCopy->setShortcut(QKeySequence::Copy);

    actionPaste =
        new QAction(QIcon(":/icons/clipboard.svg"), tr("&Paste"), this);
    actionPaste->setShortcut(QKeySequence::Paste);

    actionQuit = new QAction(QIcon(":/icons/delete.svg"), tr("&Quit"), this);
    actionQuit->setShortcut(QKeySequence::Quit);
    connect(actionQuit, &QAction::triggered, this, &QCoreApplication::quit);

    actionCut = new QAction(QIcon(":/icons/cut.svg"), tr("Cut"), this);
    actionCut->setShortcut(QKeySequence::Cut);

    actionPrint = new QAction(QIcon(":/icons/print.svg"), tr("Print"), this);
    actionPrint->setShortcut(QKeySequence::Print);

    actionPreferences =
        new QAction(QIcon(":/icons/settings.svg"), tr("Preferences"), this);
    actionPreferences->setShortcut(QKeySequence::Preferences);

    actionSticky_note_mode =
        new QAction(QIcon(":/icons/sticker.svg"), tr("Sticky note mode"), this);
    actionSticky_note_mode->setToolTip(tr("Switch to sticky note mode"));
    connect(actionSticky_note_mode, &QAction::triggered, [&]() {
        stickyNoteMode();
        actionSticky_note_mode->setDisabled(1);
        actionNormalmode->setEnabled(1);
    });

    actionNormalmode = new QAction(QIcon(":/icons/document_edit.svg"),
                                   tr("Normal mode"), this);
    actionNormalmode->setToolTip(tr("Switch to normal mode"));
    connect(actionNormalmode, &QAction::triggered, [&]() {
        normalMode(0);
        actionNormalmode->setDisabled(1);
        actionSticky_note_mode->setEnabled(1);
    });

    actionPin_to_top =
        new QAction(QIcon(":/icons/pin.svg"), tr("Pin to top"), this);
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
        new QAction(QIcon(":/icons/preview.svg"), tr("Preview panel"), this);
    actionPreview_panel->setCheckable(true);
    connect(actionPreview_panel, &QAction::triggered, [&]() {
        previewPanel->setEnabled(1);
        DockWidget->setVisible(!DockWidget->isVisible());
    });

    actionAbout = new QAction(QIcon(":/icons/info.svg"), tr("&About"), this);
    connect(actionAbout, &QAction::triggered,
            [&]() { AboutWindow(this).exec(); });

    actionAboutQt = new QAction(tr("&About Qt"), this);
    connect(actionAboutQt, &QAction::triggered, this, &QApplication::aboutQt);

    actionReadOnlyMode =
        new QAction(QIcon(":/icons/read_only.svg"), tr("Read-Only mode"), this);
    actionReadOnlyMode->setCheckable(true);
    actionReadOnlyMode->setChecked(0);

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

    menuHelp = menuBar()->addMenu(tr("&Help"));
    menuHelp->addAction(actionAbout);
    menuHelp->addAction(actionAboutQt);

    menuBar()->addMenu(menuFile);
    menuBar()->addMenu(menuEdit);
    menuBar()->addMenu(menuView);
    menuBar()->addMenu(menuHelp);
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
    QString showName;
    if (!plainTextEdit->currentFile.isEmpty())
        showName = plainTextEdit->currentFileName;
    else
        showName = "Untitled";

    for (int i = 0; i < tabData.size(); i++) {
        if (tabData[i].fileName.isEmpty())
            TabBar->setTabText(i, "Untitled");
        else
            TabBar->setTabText(i, tabData[i].fileName);
    }
    setWindowTitle(showName + "[*] - Notepanda");
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

    if (previewPanel->isEnabled()) {
        previewPanel->reload();

        const QString fileSuffix =
            plainTextEdit->currentFileName.split('.').last();

        if (fileSuffix == "md" || fileSuffix == "markdown" ||
            fileSuffix == "mdown")
            previewPanel->setMarkdown(plainTextEdit->toPlainText());
        else
            previewPanel->setText(plainTextEdit->toPlainText());
    }
}

void MainWindow::saveTabData(const int index)
{
    tabData[index].fileName = plainTextEdit->currentFileName;
    tabData[index].isModified = plainTextEdit->document()->isModified();
    tabData[index].plainText = plainTextEdit->toPlainText();
}
