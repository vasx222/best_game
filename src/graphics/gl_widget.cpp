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
  , m_background(background)
{
  setMinimumSize(1024, 768);
  setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget()
{
  makeCurrent();
  delete m_texture;
  delete m_texturedRect;
  doneCurrent();
}

void GLWidget::initializeGL()
{
  initializeOpenGLFunctions();

  m_texturedRect = new TexturedRect();
  m_texturedRect->Initialize(this);

  m_texture = new QOpenGLTexture(QImage("data/alien.png"));

  m_time.start();
}

void GLWidget::InitStars(int const starsAmount, int const starRadius, int const anglesAmount)
{
  m_starsCenters.resize(starsAmount);
  for (size_t i = 0; i < m_starsCenters.size(); i++)
  {
    m_starsCenters[i] = {rand() % this->rect().width(), rand() % this->rect().height()};
  }

  Point2D p(starRadius / sqrt(2), starRadius / sqrt(2));
  double const deltaAngle = 2 * M_PI / anglesAmount;
  for (size_t i = 0; i < anglesAmount; i++)
  {
    m_starPoints.push_back({(int)(p.x() / (1 + (i % 2))), (int)(p.y() / (1 + (i % 2)))});
    p = p.RotateCW(deltaAngle);
  }
}

void GLWidget::paintGL()
{
  int const elapsed = m_time.elapsed();
  UpdateKeys(elapsed / 1000.0f);

  QPainter painter;
  painter.begin(this);

  glClearColor(m_background.redF(), m_background.greenF(), m_background.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  painter.beginNativePainting();

  DrawStars(&painter);
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
  m_screenSize.setWidth(w);
  m_screenSize.setHeight(h);
  InitStars(100, 10, 12);
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
  m_texturedRect->Render(m_texture, m_position, QSize(128, 128), m_screenSize);
  m_texturedRect->Render(m_texture, QVector2D(400, 400), QSize(128, 128), m_screenSize);
  m_texturedRect->Render(m_texture, QVector2D(600, 600), QSize(128, 128), m_screenSize);
}

void GLWidget::DrawStars(QPainter * painter)
{
  QPoint points[m_starPoints.size()];
  for (size_t i = 0; i < m_starsCenters.size(); i++)
  {
    static double k = 0;
    k += (1.0) / 5000.0;
    if (k > 2.0 * M_PI) k -= 2.0 * M_PI;
    QColor starColor = {100, 200, 220, fabs(sin(k + i * 100 / 5000.0)) * 255.0};
    painter->setBrush(starColor);
    painter->setPen(starColor);
    for (size_t j = 0; j < m_starPoints.size(); j++)
    {
      points[j] = {m_starPoints[j].x() + m_starsCenters[i].x(), m_starPoints[j].y() + m_starsCenters[i].y()};
    }
    painter->drawPolygon(points, m_starPoints.size(), Qt::OddEvenFill/*Qt::WindingFill*/);
  }

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

  int const px = L2D(e->x());
  int const py = L2D(e->y());
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
