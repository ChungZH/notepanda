/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file texteditor.cpp
 * @brief This file implements the TextEditor class.
 *
 */
#include "texteditor.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDropEvent>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QSaveFile>
#include <QString>
#include <QStyle>
#include <QTextBlock>
#include <QTextStream>

// KSyntaxHighlighting
#include <definition.h>
#include <foldingregion.h>
#include <syntaxhighlighter.h>
#include <theme.h>

#include "../ui/linenumberarea.h"

TextEditor::TextEditor(ConfigManager *cfManager, QWidget *parent)
    : QPlainTextEdit(parent),
      configManager(cfManager),
      m_highlighter(new KSyntaxHighlighting::SyntaxHighlighter(document()))

{
    const auto theme = m_repository.theme(configManager->getEditorColorTheme());
    setTheme(theme);

    setTabStopDistance(QFontMetricsF(QFont(configManager->getEditorFontFamily(),
                                           configManager->getEditorFontSize()))
                           .horizontalAdvance(' ') *
                       configManager->getEditorTabSize());

    // Line number area
    lineNumberArea = new LineNumberArea(this);

    connect(this, &TextEditor::blockCountChanged, this,
            &TextEditor::updateLineNumberAreaWidth);
    connect(this, &TextEditor::updateRequest, this,
            &TextEditor::updateLineNumberArea);
    connect(this, &TextEditor::cursorPositionChanged, this,
            &TextEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    currentMode = 0;

    TextEditor::setFont(QFont(configManager->getEditorFontFamily(),
                              configManager->getEditorFontSize()));
    setCurrentFile(QString());
    lineNumberArea->resize(0, 0);
}

bool TextEditor::maybeSave()
{
    if (!QPlainTextEdit::document()->isModified()) return true;
    const QMessageBox::StandardButton ret = QMessageBox::warning(
        this, tr("Application"),
        tr("The document has been modified.\n"
           "Do you want to save your changes?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
        case QMessageBox::Save:
            save();
            return true;
        case QMessageBox::Cancel:
            return false;
        default:
            break;
    }
    return true;
}

bool TextEditor::newDocument()
{
    clear();
    setCurrentFile(QString());
    emit changeTitle();
    return true;
}

bool TextEditor::open()
{
    if (maybeSave()) {
        QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open the file"));
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("Cannot open file: ") + file.errorString());
            qWarning() << "[WARN 1] Failed to open" << fileName << ":"
                       << file.errorString();
            return false;
        }

        clear();

        auto def = m_repository.definitionForFileName(fileName);
        if (currentMode == 1) def = m_repository.definitionForName("Markdown");
        m_highlighter->setDefinition(def);

        QTextStream in(&file);

#ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif

        setPlainText(in.readAll());

#ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
#endif
        file.close();

        setCurrentFile(fileName);
        emit changeTitle();
        return true;
    }
    return false;
}

/**
 * @brief Open from command line like `notepanda texteditor.cpp`.
 *        It doesn't need `maybeSave()` because at this time notepanda opens for
 * the first time.
 */
void TextEditor::openFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Cannot open file: ") + file.errorString());
        qWarning() << "[WARN 2] Failed to open" << fileName << ":"
                   << file.errorString();
        return;
    }

    const auto def = m_repository.definitionForFileName(fileName);
    m_highlighter->setDefinition(def);

    QTextStream in(&file);

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    setCurrentFile(fileName);
    emit changeTitle();
}

void TextEditor::save()
{
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QString fileName, errorMessage;
    if (currentFile.isEmpty())
        fileName = QFileDialog::getSaveFileName(this, tr("Save"));
    else
        fileName = currentFile;

    QSaveFile file(fileName);

    if (file.open(QFile::WriteOnly | QFile::Text)) {
        setCurrentFile(fileName);
        emit changeTitle();

        QTextStream out(&file);
        out << toPlainText();

        if (!file.commit())
            errorMessage = "[WARN 5] Cannot save file: " + file.errorString();
    } else {
        errorMessage = "[WARN 3] Cannot save file: " + file.errorString();
    }

    if (!errorMessage.isEmpty()) {
        qWarning() << errorMessage;
        QMessageBox::warning(this, tr("Warning"),
                             tr("Cannot save file: ") + file.errorString());
    }

    QGuiApplication::restoreOverrideCursor();
}

void TextEditor::saveAs()
{
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as"));
    QSaveFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Cannot save file: ") + file.errorString());
        qWarning() << "[WARN 4] Failed to save" << fileName << ":"
                   << file.errorString();
        return;
    }

    setCurrentFile(fileName);

    emit changeTitle();
    QTextStream out(&file);
    out << toPlainText();

    QGuiApplication::restoreOverrideCursor();

    if (!file.commit()) {
        QMessageBox::warning(
            this, tr("Warning"),
            tr("Cannot save file: ") + fileName + file.errorString());
        qWarning() << "[WARN 6] Failed to save" << fileName << ":"
                   << file.errorString();
    }
}

void TextEditor::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
    QPrinter printDev;
#if QT_CONFIG(printdialog)
    QPrintDialog dialog(&printDev, this);
    if (dialog.exec() == QDialog::Rejected) return;
#endif  // QT_CONFIG(printdialog)
QPlainTextEdit:
    print(&printDev);
#endif  // QT_CONFIG(printer)
}

void TextEditor::undo() { QPlainTextEdit::undo(); }

void TextEditor::redo() { QPlainTextEdit::redo(); }

void TextEditor::copy()
{
#if QT_CONFIG(clipboard)
    QPlainTextEdit::copy();
#endif
}

void TextEditor::paste()
{
#if QT_CONFIG(clipboard)
    QPlainTextEdit::paste();
#endif
}

void TextEditor::cut()
{
#if QT_CONFIG(clipboard)
    QPlainTextEdit::cut();
#endif
}

int TextEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    return 15 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits +
           fontMetrics().lineSpacing();
}

void TextEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    if (currentMode == 1)
        setViewportMargins(0, 0, 0, 0);
    else
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void TextEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->sizeHint().width(),
                               rect.height());
    if (rect.contains(viewport()->rect())) updateLineNumberAreaWidth(0);
}

void TextEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(
        QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void TextEditor::contextMenuEvent(QContextMenuEvent *event)
{
    // if you want to **extend** the standard context menu, use
    // auto menu = createStandardContextMenu(event->pos());

    // if you want to customize the context menu, use
    QMenu *menu = new QMenu;

    // Read only
    QAction *readOnlyAct =
        new QAction(QIcon(":/icons/read_only.svg"), tr("Read-Only mode"), this);
    readOnlyAct->setCheckable(true);
    readOnlyAct->setChecked(isReadOnly());
    menu->addAction(readOnlyAct);
    connect(readOnlyAct, &QAction::triggered, [&]() {
        setReadOnly(!isReadOnly());
        readOnlyAct->setChecked(isReadOnly());
        emit readOnlyChanged();
    });

    // syntax selection
    auto hlActionGroup = new QActionGroup(menu);
    hlActionGroup->setExclusive(true);
    auto hlGroupMenu = menu->addMenu(QStringLiteral("Syntax"));
    QMenu *hlSubMenu = hlGroupMenu;
    QString currentGroup;
    for (const auto &def : m_repository.definitions()) {
        if (def.isHidden()) continue;
        if (currentGroup != def.section()) {
            currentGroup = def.section();
            hlSubMenu = hlGroupMenu->addMenu(def.translatedSection());
        }

        auto action = hlSubMenu->addAction(def.translatedName());
        action->setCheckable(true);
        action->setData(def.name());
        hlActionGroup->addAction(action);
        if (def.name() == m_highlighter->definition().name())
            action->setChecked(true);
    }
    connect(hlActionGroup, &QActionGroup::triggered, this,
            [this](QAction *action) {
                const auto defName = action->data().toString();
                const auto def = m_repository.definitionForName(defName);
                m_highlighter->setDefinition(def);
            });

    // clear
    QAction *clearAct = new QAction(tr("Clear"), this);
    menu->addAction(clearAct);
    connect(clearAct, &QAction::triggered, [&]() { newDocument(); });

    menu->exec(event->globalPos());
    delete menu;
}

void TextEditor::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Tab &&
        configManager->getEditorIndentMode() == "Spaces") {
        this->insertPlainText(QString(configManager->getEditorTabSize(), ' '));
        e->accept();
    } else {
        QPlainTextEdit::keyPressEvent(e);
    }
}

void TextEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        selection.format.setBackground(
            QColor(m_highlighter->theme().editorColor(
                KSyntaxHighlighting::Theme::CurrentLine)));
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    if (currentMode == 1) extraSelections.clear();

    setExtraSelections(extraSelections);
}

void TextEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(),
                     m_highlighter->theme().editorColor(
                         KSyntaxHighlighting::Theme::IconBorder));

    auto block = firstVisibleBlock();
    auto blockNumber = block.blockNumber();
    int top = blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + blockBoundingRect(block).height();
    const int currentBlockNumber = textCursor().blockNumber();

    const auto foldingMarkerSize = fontMetrics().lineSpacing();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            const auto number = QString::number(blockNumber + 1);
            painter.setPen(m_highlighter->theme().editorColor(
                (blockNumber == currentBlockNumber)
                    ? KSyntaxHighlighting::Theme::CurrentLineNumber
                    : KSyntaxHighlighting::Theme::LineNumbers));
            painter.drawText(0, top,
                             lineNumberArea->width() - foldingMarkerSize + 3,
                             fontMetrics().height(), Qt::AlignRight, number);
        }

        // folding marker
        if (block.isVisible() && isFoldable(block)) {
            QPolygonF polygon;
            if (isFolded(block)) {
                polygon << QPointF(foldingMarkerSize * 0.4,
                                   foldingMarkerSize * 0.25);
                polygon << QPointF(foldingMarkerSize * 0.4,
                                   foldingMarkerSize * 0.75);
                polygon << QPointF(foldingMarkerSize * 0.8,
                                   foldingMarkerSize * 0.5);
            } else {
                polygon << QPointF(foldingMarkerSize * 0.25,
                                   foldingMarkerSize * 0.4);
                polygon << QPointF(foldingMarkerSize * 0.75,
                                   foldingMarkerSize * 0.4);
                polygon << QPointF(foldingMarkerSize * 0.5,
                                   foldingMarkerSize * 0.8);
            }
            painter.save();
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(m_highlighter->theme().editorColor(
                KSyntaxHighlighting::Theme::CodeFolding)));
            painter.translate(lineNumberArea->width() - foldingMarkerSize + 3,
                              top);
            painter.drawPolygon(polygon);
            painter.restore();
        }

        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void TextEditor::dropEvent(QDropEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();

    for (auto iUrl : urls) {
        QString filename = iUrl.toLocalFile();
        qDebug() << filename;
        emit openFileInNewTab(filename);
    }

    e->accept();
}

void LineNumberArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->x() >= width() - textEditor->fontMetrics().lineSpacing()) {
        auto block = textEditor->blockAtPosition(event->y());
        if (!block.isValid() || !textEditor->isFoldable(block)) return;
        textEditor->toggleFold(block);
    }
    QWidget::mouseReleaseEvent(event);
}

QTextBlock TextEditor::blockAtPosition(int y) const
{
    auto block = firstVisibleBlock();
    if (!block.isValid()) return QTextBlock();

    int top = blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + blockBoundingRect(block).height();
    do {
        if (top <= y && y <= bottom) return block;
        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
    } while (block.isValid());
    return QTextBlock();
}

bool TextEditor::isFoldable(const QTextBlock &block) const
{
    return m_highlighter->startsFoldingRegion(block);
}

bool TextEditor::isFolded(const QTextBlock &block) const
{
    if (!block.isValid()) return false;
    const auto nextBlock = block.next();
    if (!nextBlock.isValid()) return false;
    return !nextBlock.isVisible();
}

void TextEditor::toggleFold(const QTextBlock &startBlock)
{
    // we also want to fold the last line of the region, therefore the ".next()"
    const auto endBlock =
        m_highlighter->findFoldingRegionEnd(startBlock).next();

    if (isFolded(startBlock)) {
        // unfold
        auto block = startBlock.next();
        while (block.isValid() && !block.isVisible()) {
            block.setVisible(true);
            block.setLineCount(block.layout()->lineCount());
            block = block.next();
        }

    } else {
        // fold
        auto block = startBlock.next();
        while (block.isValid() && block != endBlock) {
            block.setVisible(false);
            block.setLineCount(0);
            block = block.next();
        }
    }

    // redraw document
    document()->markContentsDirty(
        startBlock.position(), endBlock.position() - startBlock.position() + 1);

    // update scrollbars
    emit document()->documentLayout()->documentSizeChanged(
        document()->documentLayout()->documentSize());
}

void TextEditor::setTheme(const KSyntaxHighlighting::Theme &theme)
{
    auto pal = qApp->palette();
    if (theme.isValid()) {
        pal.setColor(
            QPalette::Base,
            theme.editorColor(KSyntaxHighlighting::Theme::BackgroundColor));
        pal.setColor(
            QPalette::Highlight,
            theme.editorColor(KSyntaxHighlighting::Theme::TextSelection));
    }

    setPalette(pal);

    m_highlighter->setTheme(theme);
    m_highlighter->rehighlight();
    highlightCurrentLine();
}

void TextEditor::setEditorFont(const QFont &font)
{
    QFont f = QFont(font.family(), configManager->getEditorFontSize());
    QPlainTextEdit::setFont(f);
    configManager->setEditorFontFamily(font.family());
    lineNumberArea->setFont(f);
}

void TextEditor::setEditorFontSize(const int &size)
{
    QFont font = configManager->getEditorFontFamily();
    font.setPointSize(size);
    QPlainTextEdit::setFont(font);
    configManager->setEditorFontSize(size);
}

void TextEditor::setEditorColorTheme(const QString &ctname)
{
    const auto theme = m_repository.theme(ctname);
    setTheme(theme);
}

void TextEditor::setCurrentFile(const QString &fileName)
{
    currentFile = fileName;
    if (!fileName.isEmpty()) currentFileName = QFileInfo(fileName).fileName();
    document()->setModified(false);
    setWindowModified(false);
    emit modifiedFalse();
}

void TextEditor::switchMode(const int &mode)
{
    if (mode == 0) {
        lineNumberArea->show();
        currentMode = mode;
        QPlainTextEdit::setFont(QFont(configManager->getEditorFontFamily(),
                                      configManager->getEditorFontSize()));
        setStyleSheet("background: " +
                      QString::number(m_highlighter->theme().editorColor(
                          KSyntaxHighlighting::Theme::BackgroundColor)));
        setEditorFont(configManager->getEditorFontFamily());
    } else if (mode == 1) {
        lineNumberArea->hide();
        setViewportMargins(0, 0, 0, 0);
        currentMode = mode;
        setFont(QFont(
            QFontDatabase::systemFont(QFontDatabase::GeneralFont).toString(),
            configManager->getEditorFontSize()));
        auto def = m_repository.definitionForName("Markdown");
        m_highlighter->setDefinition(def);
    }
}

// QPlainTextEdit can only zoomIn in Read-Only mode
void TextEditor::wheelEvent(QWheelEvent *e)
{
    // Qt: if (!(d->control->textInteractionFlags() & Qt::TextEditable))
    if (e->modifiers() & Qt::ControlModifier) {
        float delta = e->angleDelta().y() / 120.f;
        zoomInF(delta);
        return;
    }

    QAbstractScrollArea::wheelEvent(e);
    updateMicroFocus();
}

void TextEditor::updateSyntaxHighlight()
{
    const auto def = m_repository.definitionForFileName(currentFile);
    m_highlighter->setDefinition(def);
}
