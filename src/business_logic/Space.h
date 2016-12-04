#pragma once
#include <list>
#include "GameObject.h"
#include "Player.h"
#include "Bullet.h"
#include "Alien.h"
#include "Obstacle.h"
#include "geometry/Point2D.h"
#include <QColor>
#include <QSize>
#include <QPoint>
#include <QPainter>

class Space : public GameEntity
{
public:
  Space(Player const & player, QColor const & background, QSize const & screenSize);

  std::list <Alien> m_aliens;
  std::list <Bullet> m_bullets;
  std::list <Obstacle> m_obstacles;
  void InitStars(int const starsAmount, int const starRadius, int const anglesAmount);
  void DrawStars(QPainter * painter);

  QSize m_screenSize;
  Player m_player;
  QColor m_background;
  std::vector<QPoint> m_starsCenters;
  std::vector<QPoint> m_starPoints;
private:

};
