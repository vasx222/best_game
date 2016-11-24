#include "main_window.hpp"

#include <QApplication>
#include "gl_widget.hpp"
#include "settings_window.h"

typedef void (QWidget::*QWidgetVoidSlot)();

void MainWindow::ButtonStartClick()
{
  m_groupBox->hide();
  m_glWidget = new GLWidget(this, qRgb(20, 20, 50));
  m_timer = new QTimer(this);
  m_timer->setInterval(10);
  setCentralWidget(m_glWidget);
  connect(m_timer, &QTimer::timeout, m_glWidget, static_cast<QWidgetVoidSlot>(&QWidget::update));
  m_timer->start();
}

void MainWindow::ButtonSettingsClick()
{
  SettingsWindow * sw = new SettingsWindow(this);
  this->hide();
  sw->show();
}

MainWindow::MainWindow()
{
  this->resize(220, 120);
  QWidget * centralWidget = new QWidget(this);
  this->setCentralWidget(centralWidget);
  QGridLayout * gLayout = new QGridLayout(centralWidget);
  m_groupBox = new QGroupBox("Menu:", this);
  m_groupBox->resize(100, 120);
  gLayout->addWidget(m_groupBox);
  m_buttonStart = new QPushButton("Start game", m_groupBox);
  m_buttonSettings = new QPushButton("Settings", m_groupBox);
  QVBoxLayout * vLayout = new QVBoxLayout(m_groupBox);
  vLayout->addWidget(m_buttonStart);
  vLayout->addWidget(m_buttonSettings);

  connect(m_buttonStart, SIGNAL(clicked()), this, SLOT(ButtonStartClick()));
  connect(m_buttonSettings, SIGNAL(clicked()), this, SLOT(ButtonSettingsClick()));

  setFocusPolicy(Qt::StrongFocus);
}
