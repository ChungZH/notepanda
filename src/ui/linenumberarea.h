/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file linenumberarea.h
 * @brief This file declares the LineNumberArea class.
 *
 */
#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>

#include "../core/texteditor.h"

class LineNumberArea : public QWidget
{
    Q_OBJECT

   public:
    LineNumberArea(TextEditor *editor) : QWidget(editor), textEditor(editor) {}
    QSize sizeHint() const override
    {
        return QSize(textEditor->lineNumberAreaWidth(), 0);
    }

   protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override
    {
        textEditor->lineNumberAreaPaintEvent(event);
    }

   private:
    TextEditor *textEditor;
};

#endif  // LINENUMBERAREA_H
