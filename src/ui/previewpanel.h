/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file previewpanel.h
 * @brief This file declares the PreviewPanel class.
 *        It can preview files in Markdown, HTML and other formats.
 *
 */
#ifndef PREVIEWPANEL_H
#define PREVIEWPANEL_H

#include <QTextBrowser>

class PreviewPanel : public QTextBrowser
{
  Q_OBJECT
 public:
  PreviewPanel();
};

#endif  // PREVIEWPANEL_H
