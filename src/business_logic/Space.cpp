#include "Space.h"

Space::Space(Player const & player, QColor const & background, QSize const & screenSize):
  m_player(player), m_background(background), m_screenSize(screenSize)
{
  Logger::Instance() << "Constructor " << *this << "\n";
}

void Space::InitStars(int const starsAmount, int const starRadius, int const anglesAmount)
{
  m_starsCenters.resize(starsAmount);
  m_starPoints.clear();
  for (size_t i = 0; i < m_starsCenters.size(); i++)
  {
    m_starsCenters[i] = {rand() % this->m_screenSize.width(), rand() % this->m_screenSize.height()};
  }

  Point2D p(starRadius / sqrt(2), starRadius / sqrt(2));
  double const deltaAngle = 2 * M_PI / anglesAmount;
  for (size_t i = 0; i < anglesAmount; i++)
  {
    m_starPoints.push_back({(int)(p.x() / (1 + (i % 2))), (int)(p.y() / (1 + (i % 2)))});
    p = p.RotateCW(deltaAngle);
  }
}

void Space::DrawStars(QPainter * painter)
{
  QPoint points[m_starPoints.size()];
  for (size_t i = 0; i < m_starsCenters.size(); i++)
  {
    static double k = 0;
    k += (1.0) / 5000.0;
    if (k > 2.0 * M_PI) k -= 2.0 * M_PI;
    QColor starColor = {100, 200, 220, (int)(fabs(sin(k + i * 100 / 5000.0)) * 255.0)};
    painter->setBrush(starColor);
    painter->setPen(starColor);
    for (size_t j = 0; j < m_starPoints.size(); j++)
    {
      points[j] = {m_starPoints[j].x() + m_starsCenters[i].x(), m_starPoints[j].y() + m_starsCenters[i].y()};
    }
    painter->drawPolygon(points, m_starPoints.size(), Qt::OddEvenFill/*Qt::WindingFill*/);
  }

}
