#pragma once
#include "GameObject.h"
#include <vector>
#include <QPixmap>
#include <QPainter>
#include <QString>
#include "geometry\Point2D.h"

class ExplosionFrames : GameObject
{
public:
  void Init(QString const & fileName);
  void DrawExplosion(QPainter * painter, QPoint const & position, QSize const & size, int const curFrame);
  int const FramesAmount();
  int const Pause = 20;
private:
  std::vector<QPixmap> m_frames;
  int m_framesAmount;
};

class Explosion
{
public:
  QPoint position;
  QSize size;
  int curFrame;
  int pause;
};
