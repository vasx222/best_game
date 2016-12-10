#include "explosion.h"

void ExplosionFrames::Init(QString const & fileName)
{
  QPixmap pixmap(fileName);
  int const cols = 8, rows = 4;
  int const width = pixmap.width() / cols, height = pixmap.height() / rows;
  m_framesAmount = cols * rows;
  for (int y = 0; y < rows; y++)
  {
    for (int x = 0; x < cols; x++)
    {
      m_frames.push_back(pixmap.copy(x * width, y * height, width, height));
    }
  }
}

void ExplosionFrames::DrawExplosion(QPainter * painter, QPoint const & position, QSize const & size, int const curFrame)
{
  painter->drawPixmap(position.x() - size.width() / 2, position.y() - size.height() / 2,
     size.width(), size.height(), m_frames[curFrame]);
}

int const ExplosionFrames::FramesAmount()
{
  return m_framesAmount;
}
