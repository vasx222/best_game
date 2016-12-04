#include "gl_widget.hpp"

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

#include "main_window.hpp"
#include "geometry/Point2D.h"
#include "patterns/logger.h"

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

GLWidget::GLWidget(MainWindow * mw, QColor const & background)
  : m_mainWindow(mw)
  , m_space(Space(Player(Box2D(100, 100, 200, 200), Point2D(0, 1), 0), background, QSize(size()) ))
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

  for (int i = 0; i < beingConfigs.size(); i++)
  {
    beingConfigs[i].texture = new QOpenGLTexture(QImage(beingConfigs[i].fileName));
  }

  m_time.start();
}



void GLWidget::paintGL()
{
  int const elapsed = m_time.elapsed();
  UpdateKeys(elapsed / 1000.0f);

  QPainter painter;
  painter.begin(this);

  painter.beginNativePainting();
  glClearColor(m_space.m_background.redF(), m_space.m_background.greenF(), m_space.m_background.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_space.DrawStars(&painter);
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
  }
  painter.end();

  if (!(m_frames % 100))
  {
    m_time.start();
    m_frames = 0;
  }
  ++m_frames;
  update();
}

void GLWidget::resizeGL(int w, int h)
{
  m_space.m_screenSize.setWidth(w);
  m_space.m_screenSize.setHeight(h);
  m_space.InitStars(100, 10, 12);
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
  Player const & player = m_space.m_player;
  m_texturedRect->Render(beingConfigs[player.TypeBeing()].texture, QVector2D(player.Box().CentralPoint()), player.Box().Size(), m_space.m_screenSize);
  m_texturedRect->Render(beingConfigs[1].texture, m_position, QSize(100, 100), m_space.m_screenSize);
  m_texturedRect->Render(beingConfigs[1].texture, QVector2D(400, 400), QSize(100, 100), m_space.m_screenSize);
  m_texturedRect->Render(beingConfigs[1].texture, QVector2D(600, 600), QSize(100, 100), m_space.m_screenSize);
}

void GLWidget::mousePressEvent(QMouseEvent * e)
{
  QOpenGLWidget::mousePressEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());


  if (IsLeftButton(e))
  {
    // ...
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

  m_space.m_player.Move(Point2D(L2D(e->x()), m_space.m_screenSize.height() - L2D(e->y())));

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
    // ...
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
