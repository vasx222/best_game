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
