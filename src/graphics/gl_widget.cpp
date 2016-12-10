#include "gl_widget.hpp"

#include <QString>
#include <QPainter>
#include <QPaintEngine>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QCoreApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QGuiApplication>
#include <cmath>
#include <iostream>
#include <QtOpenGL>
#include <QOpenGLFunctions_1_5>
#include <memory>
#include <QPixmap>
#include <QImage>

#include "main_window.hpp"
#include "geometry/Point2D.h"
#include "patterns/logger.h"
#include "patterns/factory.h"
#include "business_logic/bullet.h"

namespace
{

int constexpr kLeftDirection = 0;
int constexpr kRightDirection = 1;
int constexpr kUpDirection = 2;
int constexpr kDownDirection = 3;

bool IsLeftButton(Qt::MouseButtons buttons)
{
  return buttons & Qt::LeftButton;
}
bool IsLeftButton(QMouseEvent const * const e)
{
  return IsLeftButton(e->button()) || IsLeftButton(e->buttons());
}

bool IsRightButton(Qt::MouseButtons buttons)
{
  return buttons & Qt::RightButton;
}
bool IsRightButton(QMouseEvent const * const e)
{
  return IsRightButton(e->button()) || IsRightButton(e->buttons());
}

} // namespace

GLWidget::GLWidget(MainWindow/*QWidget*/ * mw, QColor const & background)
  : m_mainWindow(mw)
  , m_space(Space(Player(Point2D(600, 400), Point2D(0, -1), 0), background, QSize(size()) ))
{
  setMinimumSize(1024, 768);
  setFocusPolicy(Qt::StrongFocus);
  this->setMouseTracking(true);
}

GLWidget::~GLWidget()
{
  makeCurrent();
  for (int i = 0; i < beingConfigs.size(); i++)
  {
    delete beingConfigs[i].texture;
  }
  delete m_texturedRect;
  doneCurrent();
}

void GLWidget::initializeGL()
{
  initializeOpenGLFunctions();

  m_texturedRect = new TexturedRect();
  m_texturedRect->Initialize(this);

  for (int i = 0; i < (int)beingConfigs.size(); i++)
  {
    beingConfigs[i].texture = new QOpenGLTexture(QImage(beingConfigs[i].fileName));
  }
  m_space.m_explosionFrames.Init("data/ready/explosion.png");

  for (int i = 0; i < (int)obstacleConfigs.size(); i++)
  {
    obstacleConfigs[i].texture = new QOpenGLTexture(QImage(obstacleConfigs[i].fileName));
  }

  for (int i = 0; i < (int)bulletConfigs.size(); i++)
  {
    bulletConfigs[i].texture = new QOpenGLTexture(QImage(bulletConfigs[i].fileName));
  }

  for (int i = 0; i < (int)itemConfigs.size(); i++)
  {
    itemConfigs[i].texture = new QOpenGLTexture(QImage(itemConfigs[i].fileName));
  }

  m_time.start();
}

void GLWidget::paintGL()
{
  if (m_space.m_player.Hp() <= 0) m_mainWindow->close();
  int const elapsed = m_time.elapsed();
  UpdateKeys(elapsed / 1000.0f);

  QPainter painter;
  painter.begin(this);
  painter.beginNativePainting();
  glClearColor(m_space.m_background.redF(), m_space.m_background.greenF(), m_space.m_background.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_space.IncTime();
  m_space.DrawStars(&painter);

  m_space.MovePlayer();
  m_space.CheckCreateAlien();
  m_space.MoveAliens();
  m_space.CheckShotPlayer();
  m_space.MoveBullets();
  m_space.CheckHitAliens();
  m_space.DrawExplosions(&painter);
  m_space.CheckCreateObstacle();
  m_space.MoveObstacles();
  m_space.CheckHitObstacles();
  m_space.CheckCollideObstacles();
  m_space.CheckCreateItem();
  m_space.MoveItems();
  m_space.CheckCollideItems();

  glFrontFace(GL_CW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Render();

  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);

  painter.endNativePainting();

  if (elapsed != 0)
  {
    QString framesPerSecond;
    framesPerSecond.setNum(m_frames / (elapsed / 1000.0), 'f', 2);
    painter.setPen(Qt::white);
    painter.drawText(20, 40, framesPerSecond + " fps");
    painter.drawText(20, 80, "bu="+QString::number(m_space.m_bullets.size()));
    painter.drawText(20, 120, "al="+QString::number(m_space.m_aliens.size()));
    painter.drawText(20, 160, "ob="+QString::number(m_space.m_obstacles.size()));
    painter.drawText(20, 200, "it="+QString::number(m_space.m_items.size()));
  }
  painter.end();

  if (!(m_frames % 100))
  {
    m_time.start();
    m_frames = 0;
  }
  ++m_frames;

  m_mainWindow->m_labelScore->setText("Score: " + QString::number(m_space.m_score));
  m_mainWindow->m_labelHp->setText("Health: " + QString::number(m_space.m_player.Hp()));
  m_mainWindow->m_labelAmmo->setText("Ammo: " + QString::number(m_space.m_player.Ammo()));
  QString gunName;
  switch (m_space.m_player.CallGun().TypeGun())
  {
  case 0:
    gunName = "Simple gun"; m_mainWindow->m_labelAmmo->hide(); break;
  case 1:
    gunName = "Laser"; m_mainWindow->m_labelAmmo->show(); break;
  default:
    gunName = "Heavy gun"; m_mainWindow->m_labelAmmo->show();
  }
  m_mainWindow->m_labelGun->setText(gunName);
  update();
}

void GLWidget::resizeGL(int w, int h)
{
  m_space.m_screenSize.setWidth(w);
  m_space.m_screenSize.setHeight(h);
  m_space.InitStars(100, 7, 12);
}

void GLWidget::UpdateKeys(float elapsedSeconds)
{
  float const kSpeed = 20.0f; // pixels per second.

  if (m_directions[kUpDirection])
    m_position.setY(m_position.y() + kSpeed * elapsedSeconds);
  if (m_directions[kDownDirection])
    m_position.setY(m_position.y() - kSpeed * elapsedSeconds);
  if (m_directions[kLeftDirection])
    m_position.setX(m_position.x() - kSpeed * elapsedSeconds);
  if (m_directions[kRightDirection])
    m_position.setX(m_position.x() + kSpeed * elapsedSeconds);
}

void GLWidget::Render()
{
  m_space.DrawBullets(m_texturedRect);
  Player const & player = m_space.m_player;
  QVector2D position(player.Box().CentralPoint().x(),m_space.m_screenSize.height() - player.Box().CentralPoint().y());
  m_texturedRect->Render(beingConfigs[player.TypeBeing()].texture, position,
      player.Box().Size(), m_space.m_screenSize, 0);
  m_space.DrawAliens(m_texturedRect);
  m_space.DrawObstacles(m_texturedRect);
  m_space.DrawItems(m_texturedRect);
}

void GLWidget::mousePressEvent(QMouseEvent * e)
{
  QOpenGLWidget::mousePressEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());


  if (IsLeftButton(e))
  {
    m_space.m_leftButtonPressed = true;
  }
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent * e)
{
  QOpenGLWidget::mouseDoubleClickEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());
  if (IsRightButton(e))
  {
    // ...
  }
}

void GLWidget::mouseMoveEvent(QMouseEvent * e)
{
  QOpenGLWidget::mouseMoveEvent(e);

  m_space.m_cursorPosition = Point2D(L2D(e->x()), L2D(e->y()));
  if (IsLeftButton(e))
  {
    // ...
  }
}

void GLWidget::mouseReleaseEvent(QMouseEvent * e)
{
  QOpenGLWidget::mouseReleaseEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());
  if (IsLeftButton(e))
  {
    m_space.m_leftButtonPressed = false;
  }
}

void GLWidget::wheelEvent(QWheelEvent * e)
{
  QOpenGLWidget::wheelEvent(e);

  int const delta = e->delta();
  int const px = L2D(e->x());
  int const py = L2D(e->y());
  // ...
}

void GLWidget::keyPressEvent(QKeyEvent * e)
{
  if (e->key() == Qt::Key_Escape) m_mainWindow->close();
  if (e->key() == Qt::Key_Up)
    m_directions[kUpDirection] = true;
  else if (e->key() == Qt::Key_Down)
    m_directions[kDownDirection] = true;
  else if (e->key() == Qt::Key_Left)
    m_directions[kLeftDirection] = true;
  else if (e->key() == Qt::Key_Right)
    m_directions[kRightDirection] = true;
}

void GLWidget::keyReleaseEvent(QKeyEvent * e)
{
  if (e->key() == Qt::Key_Up)
    m_directions[kUpDirection] = false;
  else if (e->key() == Qt::Key_Down)
    m_directions[kDownDirection] = false;
  else if (e->key() == Qt::Key_Left)
    m_directions[kLeftDirection] = false;
  else if (e->key() == Qt::Key_Right)
    m_directions[kRightDirection] = false;
}
