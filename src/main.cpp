#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>
#include "graphics/main_window.hpp"
#include "patterns/logger.h"
#include <fstream>

int main(int argc, char ** argv)
{
  Logger::InitLogFile();

  QApplication a(argc, argv);

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  QSurfaceFormat::setDefaultFormat(format);

  MainWindow mw;
  mw.show();
  return a.exec();
}
