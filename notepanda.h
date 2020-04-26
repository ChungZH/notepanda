#ifndef NOTEPANDA_H
#define NOTEPANDA_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class notepanda; }
QT_END_NAMESPACE

class notepanda : public QMainWindow
{
    Q_OBJECT

public:
    notepanda(QWidget *parent = nullptr);
    ~notepanda();

private slots:
    void newDocument();
    void open();
    void save();
    void saveAs();
    void about();

private:
    Ui::notepanda *ui;
    QString currentFile;
};
#endif // NOTEPANDA_H
