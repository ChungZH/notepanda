/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file mainwindow.h
 * @brief This file declares the MainWindow class.
 *        It is the main window of notepanda.
 *
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QColorDialog>
#include <QDockWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QTextBrowser>
#include <QToolBar>
#include <QVector>

#include "../core/configmanager.h"
#include "../core/texteditor.h"
#include "preferenceswindow.h"

QT_BEGIN_NAMESPACE
class TextEditor;
QT_END_NAMESPACE

struct TabData
{
    QString plainText;
    QString fileName;
    bool isModified;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

   public:
    MainWindow(ConfigManager *confManager, QWidget *parent = nullptr);
    ~MainWindow();
    TextEditor *plainTextEdit;

   protected:
    void closeEvent(QCloseEvent *event) override;

   private slots:
    void changeWindowTitle();
    void updateStatusBar();
    void normalMode(bool isFirst);
    void stickyNoteMode();
    void documentWasModified();

   private:
    void setupUi();

    // TODO: use QTabBar::tabData
    QVector<TabData> tabData;

    QToolBar *ToolBar;
    ConfigManager *configManager;
    PreferencesWindow *pfWindow;
    QTextBrowser *previewPanel;
    QDockWidget *DockWidget;
    QTabBar *TabBar;
    QToolBar *TabToolBar;

    bool lastTabRemoveFlag, doNotSaveDataFlag;
    int curTabIndex, prevTabIndex;
    void saveTabData(const int index);

    /**
     * @brief currentMode
     *    0: normal mode
     *    1: sticky mode
     */
    int currentMode;
    bool isPintotop;

    // Sticky note mode

    QToolBar *SToolBar;
    QAction *changeBgColor;
    QColorDialog *ColorDialog;
    QColor currentColor;

    // SNM end

    // UI start
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionQuit;
    QAction *actionCut;
    QAction *actionPrint;
    QAction *actionPreferences;
    QAction *actionSticky_note_mode;
    QAction *actionNormalmode;
    QAction *actionPin_to_top;
    QAction *actionPreview_panel;
    QAction *actionReadOnlyMode;
    QAction *actionAbout;
    QAction *actionAboutQt;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuHelp;
    // UI end
};
#endif  // MAINWINDOW_H
