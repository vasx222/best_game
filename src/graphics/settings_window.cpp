#include "settings_window.h"

void SettingsWindow::ButtonSave()
{
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

  m_spinBoxSpeed = new QSpinBox(m_groupBox);
  m_spinBoxSpeed->setRange(1, 100);
  m_spinBoxSpeed->setValue(40);
  m_spinBoxSpeed->setSuffix(" speed");

  m_buttonSave = new QPushButton("Save and close", m_groupBox);

  QFormLayout * fLayout = new QFormLayout(m_groupBox);
  fLayout->addRow("Difficulty:", m_comboBoxDifficulty);
  fLayout->addRow("Speed:", m_spinBoxSpeed);
  fLayout->addWidget(m_buttonSave);
  fLayout->setFormAlignment(Qt::AlignCenter);
  fLayout->setVerticalSpacing(15);

  connect(m_buttonSave, SIGNAL(clicked()), this, SLOT(ButtonSave()));

  setFocusPolicy(Qt::StrongFocus);
}
