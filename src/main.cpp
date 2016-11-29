#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>
#include "graphics/main_window.hpp"
#include "patterns/logger.h"
#include <fstream>
#include <json/json.h>
#include <json/writer.h>
#include "graphics/gameSettings.h"

#define JSONFILENAME "settings.json"

void JsonWriteToFile()
{
  Json::Value settings;
  settings["settings"]["difficultyIndex"] = gameSettings.m_difficulty;
  settings["settings"]["gameSpeed"] = gameSettings.m_gameSpeed;

  std::ofstream settingsFile;
  settingsFile.open(JSONFILENAME);
  if (settingsFile.is_open())
  {
    Json::StyledWriter styledWriter;
    settingsFile << styledWriter.write(settings);
    settingsFile.close();
  }
}

void JsonReadFromFile()
{
  Json::Value settings;
  std::ifstream settingsFile(JSONFILENAME);
  if (settingsFile.is_open())
  {
    settingsFile >> settings;
    settingsFile.close();
  }

  gameSettings.m_difficulty = settings["settings"]["difficultyIndex"].asInt();
  gameSettings.m_gameSpeed = settings["settings"]["gameSpeed"].asInt();
}

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
  JsonReadFromFile();
  int f = a.exec();
  JsonWriteToFile();
  return f;
}
