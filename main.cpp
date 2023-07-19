/**
 * @file main.cpp
 * @brief Main class for the Qt interface, which is required when interface is compiled
 * @bug no known bugs
 */

/** -- Includes -- **/
#include "mainwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  MainWindow mainWindow;
  mainWindow.showMaximized();
  return app.exec();
}
