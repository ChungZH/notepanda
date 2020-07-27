/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file main.cpp
 * @brief This file is main of the project.
 *
 */

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QStyleFactory>
#include <string>

#include "./core/configmanager.h"
#include "./ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication App(argc, argv);

    App.setOrganizationName("ChungZH");
    App.setApplicationName("Notepanda");
    App.setApplicationVersion("0.1.4");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source", "The source file to open.");
    QCommandLineOption configFileOption("c", "specify configuration file.",
                                        "config.json");
    parser.addOption(configFileOption);
    parser.process(App);

    QString configFile = parser.value(configFileOption);
    if (configFile.isEmpty()) {
#ifdef Q_OS_WIN
        configFile = App.applicationDirPath() + "/config.json";
#else
        // For Scoop package
        QDir currentPathDir = QApplication::applicationDirPath() + "/config";
        QDir configDir = QDir::homePath() + "/.config/notepanda";

        if (QDir(currentPathDir).exists()) {
            configFile = currentPathDir.absolutePath() + "/config.json";
        } else {
            configFile = configDir.absolutePath() + "/config.json";
            if (!configDir.exists()) {
                configDir.mkpath(configDir.absolutePath());
            }
        }

#endif
    }
    ConfigManager *configManager;
    configManager = new ConfigManager(configFile);

    App.setStyle(QStyleFactory::create(configManager->getStyleTheme()));

    MainWindow notepanda(configManager);
    notepanda.show();
    if (parser.positionalArguments().size() == 1)
        notepanda.plainTextEdit->openFile(parser.positionalArguments().at(0));

    qInfo() << QStringLiteral("Welcome to Notepanda!") << "";

    return App.exec();
}
