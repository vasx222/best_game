#pragma once
#include <list>
#include <QColor>
#include <QSize>
#include <QPoint>
#include <QPainter>
#include <memory>
#include "GameObject.h"
#include "Player.h"
#include "Bullet.h"
#include "Alien.h"
#include "Obstacle.h"
#include "geometry/Point2D.h"
#include "graphics/textured_rect.hpp"
#include "business_logic/explosion.h"
#include "business_logic/item.h"

class Alien;

class Space : public GameEntity
{
public:
  Space(Player const & player, QColor const & background, QSize const & screenSize);

  std::list <std::unique_ptr<Alien> > m_aliens;
  std::list <std::unique_ptr<Bullet> > m_bullets;
  std::list <std::unique_ptr<Obstacle> > m_obstacles;
  std::list <std::unique_ptr<Item> > m_items;
  std::list <Explosion> m_explosions;

  bool Probability(int const percent);

  void InitStars(int const starsAmount, int const starRadius, int const anglesAmount);
  void DrawStars(QPainter * painter);

  void MoveBullets();
  void MovePlayer();
  void MoveAliens();
  void MoveObstacles();
  void MoveItems();

  void CheckCreateAlien();
  void CheckCreateObstacle();
  void CheckCreateItem();

  void CheckShotPlayer();
  void CheckHitAliens();
  void CheckHitObstacles();

  void CheckCollideObstacles();
  void CheckCollideItems();

  void DrawAliens(TexturedRect * texturedRect);
  void DrawExplosions(QPainter * painter);
  void DrawObstacles(TexturedRect * texturedRect);
  void DrawBullets(TexturedRect * texturedRect);
  void DrawItems(TexturedRect * texturedRect);

  ExplosionFrames m_explosionFrames;
  QSize m_screenSize;
  Player m_player;
  QColor m_background;
  std::vector<QPoint> m_starsCenters;
  std::vector<QPoint> m_starPoints;
  Point2D m_cursorPosition;
  bool m_leftButtonPressed = false;
  void IncTime() {m_time = (m_time + m_deltaTime);}
  int const Time() {return m_time;}
  int m_score = 0;
private:
  int m_time = 0;
  int const m_deltaTime = 1;
};
