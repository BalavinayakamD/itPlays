/**
 * @file main.cpp
 * @brief Entry point for the itPlays video player application.
 *
 * This file initializes the Qt application and creates the main window.
 */

#include "mainwindow.h"

#include <QApplication>

/**
 * @brief Application entry point.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return Exit code from the Qt event loop.
 */
int main(int argc, char *argv[])
{
    // Create the Qt application instance
    QApplication a(argc, argv);

    // Create and display the main window
    MainWindow w;
    w.show();

    // Start the Qt event loop and return its exit code
    return a.exec();
}
