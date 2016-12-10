#include "main_window.hpp"

#include <QApplication>
#include "gl_widget.hpp"
#include "settings_window.h"
#include "business_logic/Global_configs.h"

typedef void (QWidget::*QWidgetVoidSlot)();

void MainWindow::ButtonStartClick()
{    
  InitConfig();
  m_groupBox->hide();
  this->showFullScreen();
  QPalette palette = this->palette();
  palette.setColor(QPalette::Background, QColor(Qt::black));
  this->setPalette( palette );

  QWidget * centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  m_widgetBorder1 = new QWidget(this);
  m_widgetBorder1->setFixedWidth(120);
  m_widgetBorder2 = new QWidget(this);
  m_widgetBorder2->setFixedWidth(120);

  QGridLayout * layout = new QGridLayout(centralWidget);
  layout->addWidget(m_widgetBorder1, 0, 0);
  m_glWidget = new GLWidget(this, qRgb(20, 20, 50));
  m_glWidget->setFixedHeight(this->height() - 30);
  layout->addWidget(m_glWidget, 0, 1);
  layout->addWidget(m_widgetBorder2, 0, 2);

  m_labelScore = new QLabel("Score: 0", m_widgetBorder1);
  m_labelScore->setStyleSheet("QLabel { background-color : black; color : blue; font-size: 18px}");
  m_labelScore->setGeometry(0, 0, 100, 50);
  m_labelHp = new QLabel("Health: 200", m_widgetBorder1);
  m_labelHp->setStyleSheet("QLabel { background-color : black; color : red; font-size: 18px}");
  m_labelHp->setGeometry(0, 40, 100, 50);
  m_labelGun = new QLabel("Simple gun", m_widgetBorder1);
  m_labelGun->setStyleSheet("QLabel { background-color : black; color : green; font-size: 18px}");
  m_labelGun->setGeometry(0, 80, 100, 50);
  m_labelAmmo = new QLabel("Ammo: inf", m_widgetBorder1);
  m_labelAmmo->setStyleSheet("QLabel { background-color : black; color : blue; font-size: 18px}");
  m_labelAmmo->setGeometry(0, 120, 100, 50);

  m_timer = new QTimer(this);
  m_timer->setInterval(10);
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
