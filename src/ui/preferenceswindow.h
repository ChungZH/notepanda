#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include <QObject>
#include <QWidget>

#include "ui_preferenceswindow.h"

class PreferencesWindow
        : public QDialog
        , private Ui::PreferencesWindow
{
    Q_OBJECT
public:
    PreferencesWindow(QWidget *parent = nullptr);

   private:
    Ui::PreferencesWindow *ui;
};

#endif // PREFERENCESWINDOW_H
