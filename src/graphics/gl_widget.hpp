#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTime>
#include "textured_rect.hpp"
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QSize>
#include <QVector2D>
#include <QPaintEngine>
#include <array>
#include <QWidget>

#include "business_logic/Space.h"

class MainWindow;

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(QOpenGLShader)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT
public:
  GLWidget(MainWindow/*QWidget*/ * mw, QColor const & background);
  ~GLWidget();

protected:
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void initializeGL() override;

  void UpdateKeys(float elapsedSeconds);
  void Render();

  void mousePressEvent(QMouseEvent * e) override;
  void mouseDoubleClickEvent(QMouseEvent * e) override;
  void mouseMoveEvent(QMouseEvent * e) override;
  void mouseReleaseEvent(QMouseEvent * e) override;
  void wheelEvent(QWheelEvent * e) override;
  void keyPressEvent(QKeyEvent * e) override;
  void keyReleaseEvent(QKeyEvent * e) override;

private:
  int L2D(int px) const { return px * devicePixelRatio(); }

  MainWindow * m_mainWindow;

  unsigned int m_frames = 0;
  QTime m_time;

  TexturedRect * m_texturedRect = nullptr;

  QVector2D m_position = QVector2D(200, 200);
  std::array<bool, 4> m_directions = {{ false, false, false, false }};

  Space m_space;
};
