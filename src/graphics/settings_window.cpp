#include "settings_window.h"
#include <fstream>
#include <string>
#include <iostream>
#include "gamesettings.h"

//void SettingsWindow::JsonWriteToFile()
//{
//  Json::Value settings;
//  //auto & settings = m_mainWindow->m_settings;
//  //settings.clear();
//  Json::Value difficulties(Json::arrayValue);
//  difficulties.append(Json::Value("Easy"));
//  difficulties.append(Json::Value("Medium"));
//  difficulties.append(Json::Value("Hard"));
//  auto & root = settings["settings"];
//  root["difficulties"] = difficulties;
//  root["difficultyIndex"] = m_comboBoxDifficulty->currentIndex();
//  root["gameSpeed"] = m_spinBoxSpeed->value();

//  std::ofstream settingsFile;
//  settingsFile.open("settings.json"/*JSONFILENAME*/);
//  if (settingsFile.is_open())
//  {
//    Json::StyledWriter styledWriter;
//    settingsFile << styledWriter.write(settings);
//    settingsFile.close();
//  }
//}

//void SettingsWindow::JsonReadFromFile()
//{
//  Json::Value settings;
//  //auto & settings = m_mainWindow->m_settings;
//  //settings.clear();
//  std::ifstream settingsFile("settings.json"/*JSONFILENAME*/);
//  if (settingsFile.is_open())
//  {
//    settingsFile >> settings;
//    settingsFile.close();
//  }

//  Json::Value & difficulties = settings["settings"]["difficulties"];
//  if (difficulties.isArray())
//  {
//    for (Json::Value::ArrayIndex i = 0; i < difficulties.size(); i++)
//    {
//     // m_comboBoxDifficulty->addItem(QString((difficulties[i].asString()).c_str()));
//    }
//  }
//  m_comboBoxDifficulty->setCurrentIndex(settings["settings"]["difficultyIndex"].asInt());
//  m_spinBoxSpeed->setValue(settings["settings"]["gameSpeed"].asInt());
//}

void SettingsWindow::ButtonSave()
{
  //JsonWriteToFile();
  gameSettings.m_difficulty = m_comboBoxDifficulty->currentIndex();
  gameSettings.m_gameSpeed = m_spinBoxSpeed->value();
  m_mainWindow->show();
  this->close();
}

SettingsWindow::SettingsWindow(MainWindow * mainWindow)
{
  m_mainWindow = mainWindow;

  this->setFixedSize(220, 170);
  QWidget * centralWidget = new QWidget(this);
  this->setCentralWidget(centralWidget);
  QGridLayout * gLayout = new QGridLayout(centralWidget);

  m_groupBox = new QGroupBox("Settings:", this);
  m_groupBox->resize(100, 150);
  gLayout->addWidget(m_groupBox);

  m_comboBoxDifficulty = new QComboBox(m_groupBox);
  m_comboBoxDifficulty->addItem("Easy", 1);
  m_comboBoxDifficulty->addItem("Medium", 2);
  m_comboBoxDifficulty->addItem("Hard", 3);
  m_comboBoxDifficulty->setCurrentIndex(gameSettings.m_difficulty);

  m_spinBoxSpeed = new QSpinBox(m_groupBox);
  m_spinBoxSpeed->setRange(1, 100);
  m_spinBoxSpeed->setSuffix(" speed");
  m_spinBoxSpeed->setValue(gameSettings.m_gameSpeed);

  m_buttonSave = new QPushButton("Save and close", m_groupBox);

  //JsonReadFromFile();

  QFormLayout * fLayout = new QFormLayout(m_groupBox);
  fLayout->addRow("Difficulty:", m_comboBoxDifficulty);
  fLayout->addRow("Speed:", m_spinBoxSpeed);
  fLayout->addWidget(m_buttonSave);
  fLayout->setFormAlignment(Qt::AlignCenter);
  fLayout->setVerticalSpacing(15);

  connect(m_buttonSave, SIGNAL(clicked()), this, SLOT(ButtonSave()));

  setFocusPolicy(Qt::StrongFocus);
}
