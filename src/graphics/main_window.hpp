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

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  QWidget * m_widgetBorder1 = nullptr;
  QWidget * m_widgetBorder2 = nullptr;
  QLabel * m_labelHp = nullptr;
  QLabel * m_labelGun = nullptr;
  QLabel * m_labelAmmo = nullptr;
  QLabel * m_labelScore = nullptr;

public slots:
  void ButtonStartClick();
  void ButtonSettingsClick();

private:
  QPushButton * m_buttonStart = nullptr;
  QPushButton * m_buttonSettings = nullptr;
  QTimer * m_timer = nullptr;
  QGridLayout * m_layout = nullptr;
  QOpenGLWidget * m_glWidget = nullptr;
  QGroupBox * m_groupBox = nullptr;
};
