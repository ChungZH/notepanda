#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H


#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QFileDialog;
QT_END_NAMESPACE

//![codeeditordefinition]

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    TextEditor(QWidget *parent = nullptr);


public slots:
    void newDocument();
    void open();
    void save();
    void saveAs();

private:
    QString currentFile;
};

#endif // TEXTEDITOR_H
