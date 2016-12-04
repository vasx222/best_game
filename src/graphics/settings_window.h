#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QGridLayout>
#include <QOpenGLWidget>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QFormLayout>

#include "main_window.hpp"

class SettingsWindow : public QMainWindow
{
  Q_OBJECT

public:
  SettingsWindow(MainWindow * mainWindow);
  //#define JSONFILENAME "settings.json"
//  void JsonReadFromFile();
//  void JsonWriteToFile();

public slots:
  void ButtonSave();

private:
  MainWindow * m_mainWindow;
  QComboBox * m_comboBoxDifficulty = nullptr;
  QGroupBox * m_groupBox = nullptr;
  QSpinBox * m_spinBoxSpeed = nullptr;
  QPushButton * m_buttonSave = nullptr;
};
