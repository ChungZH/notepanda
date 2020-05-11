#include "settings.h"

#include <QApplication>

QSettings settings(QApplication::organizationName(),
                   QApplication::applicationName());

Settings::Settings() {}
