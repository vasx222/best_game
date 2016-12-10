#include "Space.h"
#include <memory>
#include "bullet.h"
#include "Global_configs.h"
#include <ctime>
#include "patterns/factory.h"
#include <QVector2D>
#include <QMessageBox>
#include "geometry/Point2D.h"
#include "graphics/gamesettings.h"

Space::Space(Player const & player, QColor const & background, QSize const & screenSize):
  m_player(player), m_background(background), m_screenSize(screenSize)
{
  Logger::Instance() << "Constructor " << *this << "\n";
  srand(time(0));
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

void Space::MoveBullets()
{
  Logger::Instance() << "move_bullets\n";
  for (auto it = m_bullets.begin(); it != m_bullets.end();)
  {
    Point2D centralPoint = Point2D((*it)->Box().CentralPoint());
    int speed = bulletConfigs[(*it)->TypeBullet()].speed;
    (*it)->Move(centralPoint + (*it)->Direction() * speed);
    if (centralPoint.x() < 0 || centralPoint.y() < 0 ||
        centralPoint.x() > m_screenSize.width() || centralPoint.y() > m_screenSize.height())
    {
      it = m_bullets.erase(it);
    } else ++it;
  }
}

void Space::DrawBullets(TexturedRect * texturedRect)
{
  Logger::Instance() << "draw_bullets\n";
  for (auto it = m_bullets.begin(); it != m_bullets.end(); ++it)
  {
    QVector2D position((*it)->Box().CentralPoint().x(),
       m_screenSize.height() - (*it)->Box().CentralPoint().y());
    QSize size((*it)->Box().Size());
    texturedRect->Render(bulletConfigs[(*it)->TypeBullet()].texture, position, size, m_screenSize, 0);
  }
}

void Space::MovePlayer()
{
  m_player.SetSpeed(m_player.Speed() + beingConfigs[m_player.TypeBeing()].acceleration);
  if (m_player.Speed() > beingConfigs[m_player.TypeBeing()].maxSpeed)
  {
    m_player.SetSpeed(beingConfigs[m_player.TypeBeing()].maxSpeed);
  }
  Point2D centralPoint(m_player.Box().CentralPoint());
  Point2D deltaVector = m_cursorPosition - centralPoint;
  if (deltaVector.Length() >= m_player.Speed())
  {
    deltaVector = deltaVector.Normalize() * m_player.Speed();
    m_player.Move(centralPoint + deltaVector);
  } else
  {
    m_player.Move(m_cursorPosition);
    m_player.SetSpeed(m_player.Speed() - beingConfigs[m_player.TypeBeing()].acceleration);
    if (m_player.Speed() < beingConfigs[m_player.TypeBeing()].speed0)
    {
      m_player.SetSpeed(beingConfigs[m_player.TypeBeing()].speed0);
    }
  }
}

void Space::CheckShotPlayer()
{
  if (m_player.CallGun().Pause() > 0) m_player.CallGun().SetPause(m_player.CallGun().Pause() - 10);
  if (m_player.CallGun().Pause() == 0 && m_leftButtonPressed)
  {
    m_player.CallGun().SetPause(gunConfigs[m_player.CallGun().TypeGun()].delay);
    Point2D shotPoint(m_player.Box().PCentralPoint() + m_player.Direction() * m_player.Box().Height() / 2
                      - m_player.Direction() * 10);
    m_player.CallGun().Shot(shotPoint, Point2D(0, -1), m_bullets);
    if (m_player.CallGun().TypeGun() == 5)
    {
      m_player.CallGun().Shot(shotPoint, Point2D(0, -1).RotateCW(10 * M_PI / 180), m_bullets);
      m_player.CallGun().Shot(shotPoint, Point2D(0, -1).RotateCCW(10 * M_PI / 180), m_bullets);
    }

    if (m_player.Ammo() > 0) m_player.SetAmmo(m_player.Ammo() - 1);
    if (m_player.Ammo() == 0)
    {
      m_player.SetGun(0);
      m_player.SetAmmo(-1);
    }
  }
}

void Space::CheckCreateAlien()
{
  Logger::Instance() << "check_create_alien\n";
  int typeBeing = -1;
  int t = 300 - gameSettings.m_difficulty * 50;
  if (Time() % t == 0 && Time() >= t && Probability(90)) typeBeing = Probability(60) ? 1 : 2;
  //if (Time() % 500 == 0 && Time() >= 500) typeBeing = 3;

  if (typeBeing != -1)
  {
    int width = beingConfigs[typeBeing].width;
    int height = beingConfigs[typeBeing].height;
    Point2D position(rand() % (m_screenSize.width() - 2 * width) + width, -height * 2);
    m_aliens.push_back(Factory::Create<Alien>(position, Point2D(0, 1), typeBeing));
  }
}

void Space::MoveAliens()
{
  Logger::Instance() << "move_aliens\n";
  for (auto it = m_aliens.begin(); it != m_aliens.end();)
  {
    if ((*it)->Box().PCentralPoint().y() - (*it)->Box().Height() / 2 >= m_screenSize.height())
    {
      m_player.SetHp(m_player.Hp() - 20);
      it = m_aliens.erase(it);
    } else
    {
      (*it)->Behavior(*this);
      it++;
    }
  }
}

bool Space::Probability(int const percent)
{
  return (rand() % 100) < percent;
}

void Space::CheckHitAliens()
{
  Logger::Instance() << "check_hit_aliens\n";
  for (auto bullet = m_bullets.begin(); bullet != m_bullets.end();)
  {
    if ((*bullet)->Owner() != 0 && (*bullet)->Box().IntersectBox(m_player.Box()))
    {
      m_player.SetHp(m_player.Hp() - bulletConfigs[(*bullet)->TypeBullet()].damage);
      if (m_player.Hp() <= 0)
      {
        QPoint position(m_player.Box().CentralPoint());
        QSize size(m_player.Box().Size());
        m_explosions.push_back({position, size, 0, m_explosionFrames.Pause});
      }
      bullet = m_bullets.erase(bullet);
      continue;
    }

    bool hit = false;
    for (auto alien = m_aliens.begin(); alien != m_aliens.end();)
    {
      if ((*bullet)->Box().IntersectBox((*alien)->Box()) && (*bullet)->Owner() == 0)
      {
        hit = true;
        (*alien)->Hit((*bullet)->TypeBullet());
        if ((*alien)->Hp() <= 0)
        {
          m_score += beingConfigs[(*alien)->TypeBeing()].score;
          QPoint position((*alien)->Box().CentralPoint());
          QSize size((*alien)->Box().Size());
          m_explosions.push_back({position, size, 0, m_explosionFrames.Pause});
          m_aliens.erase(alien);
        }
        bullet = m_bullets.erase(bullet);
        break;
      }
      alien++;
    }
    if (!hit) bullet++;
  }
}

void Space::DrawExplosions(QPainter * painter)
{
  for (auto it = m_explosions.begin(); it != m_explosions.end();)
  {
    m_explosionFrames.DrawExplosion(painter, (*it).position, (*it).size, (*it).curFrame);
    (*it).pause -= 10;
    if ((*it).pause <= 0)
    {
      (*it).pause = m_explosionFrames.Pause;
      (*it).curFrame++;
    }
    if ((*it).curFrame == m_explosionFrames.FramesAmount()) it = m_explosions.erase(it);
    else it++;
    }
}

void Space::CheckCreateObstacle()
{
  if (Time() % 250 == 0 && Time() >= 250 && Probability((int)m_obstacles.size() > 7 ? 50 : 90))
  {
    int typeObstacle = rand() % 2;
    int width = obstacleConfigs[typeObstacle].minWidth + rand() %
        (obstacleConfigs[typeObstacle].maxWidth - obstacleConfigs[typeObstacle].minWidth);
    int sgn = rand() % 2 == 0 ? -1 : 1;
    int x = sgn < 0 ? -width : width + m_screenSize.width();
    int y = width + rand() % (m_screenSize.height() / 2);
    Point2D position(x, y);
    Point2D vector(0, 1);
    double angle = 55 + rand() % (125 - 55);
    if (sgn < 0) vector = vector.RotateCW(angle * M_PI / 180.0);
    else vector = vector.RotateCCW(angle * M_PI / 180.0);
    m_obstacles.push_back(Factory::Create<Obstacle>(position, vector, width, typeObstacle));
  }
}

void Space::MoveObstacles()
{
  for (auto it = m_obstacles.begin(); it != m_obstacles.end();)
  {
    if ((*it)->Box().PointMin().x() > 0 && (*it)->Box().PointMin().y() > 0 &&
        (*it)->Box().PointMax().x() < m_screenSize.width() && (*it)->Box().PointMax().y() < m_screenSize.height())
    {
      (*it)->SetInside(true);
    } else {
      if ((*it)->Inside() && ((*it)->Box().PointMax().x() < 0 || (*it)->Box().PointMax().y() < 0 ||
          (*it)->Box().PointMin().x() >= m_screenSize.width() || (*it)->Box().PointMin().y() >= m_screenSize.height()))
      {
        it = m_obstacles.erase(it);
        continue;
      }
    }

    (*it)->Rotate();
    Point2D centralPoint = (*it)->Box().PCentralPoint();
    double speed = (*it)->Speed();
    (*it)->Move(centralPoint + (*it)->Direction() * speed);
    it++;
  }
}

void Space::DrawObstacles(TexturedRect * texturedRect)
{
  for (auto it = m_obstacles.begin(); it != m_obstacles.end(); ++it)
  {
    QVector2D position((*it)->Box().CentralPoint().x(),
       m_screenSize.height() - (*it)->Box().CentralPoint().y());
    texturedRect->Render(obstacleConfigs[(*it)->TypeObstacle()].texture, position, (*it)->Box().Size(), m_screenSize,
        (*it)->Angle());
    }
}

void Space::CheckHitObstacles()
{
  Logger::Instance() << "check_hit_obstacles\n";
  for (auto bullet = m_bullets.begin(); bullet != m_bullets.end();)
  {
    bool hit = false;
    for (auto obstacle = m_obstacles.begin(); obstacle != m_obstacles.end();)
    {
      if ((*bullet)->Box().IntersectBox((*obstacle)->Box()))
      {
        hit = true;
        (*obstacle)->Hit((*bullet)->TypeBullet());
        if ((*obstacle)->Hp() <= 0)
        {
          QPoint position((*obstacle)->Box().CentralPoint());
          QSize size((*obstacle)->Box().Size());
          m_explosions.push_back({position, size, 0, m_explosionFrames.Pause});
          m_obstacles.erase(obstacle);
        }
        bullet = m_bullets.erase(bullet);
        break;
      }
      obstacle++;
    }
    if (!hit) bullet++;
    }
}

void Space::CheckCollideObstacles()
{
  Logger::Instance() << "check_collide_obstacles\n";
  for (auto obstacle = m_obstacles.begin(); obstacle != m_obstacles.end();)
  {
    if ((*obstacle)->Box().IntersectBox(m_player.Box()))
    {
      m_player.SetHp(m_player.Hp() - obstacleConfigs[(*obstacle)->TypeObstacle()].damage);
      if (m_player.Hp() <= 0)
      {
        QPoint position(m_player.Box().CentralPoint());
        QSize size(m_player.Box().Size());
        m_explosions.push_back({position, size, 0, m_explosionFrames.Pause});
      }

      QPoint position((*obstacle)->Box().CentralPoint());
      QSize size((*obstacle)->Box().Size());
      m_explosions.push_back({position, size, 0, m_explosionFrames.Pause});
      obstacle = m_obstacles.erase(obstacle);
      continue;
    }
    bool hit = false;
    for (auto alien = m_aliens.begin(); alien != m_aliens.end();)
    {
      if ((*obstacle)->Box().IntersectBox((*alien)->Box()))
      {
        QPoint position((*obstacle)->Box().CentralPoint());
        QSize size((*obstacle)->Box().Size());
        m_explosions.push_back({position, size, 0, m_explosionFrames.Pause});
        obstacle = m_obstacles.erase(obstacle);

        (*alien)->Hit(obstacleConfigs[(*obstacle)->TypeObstacle()].damage);
        if ((*alien)->Hp() <= 0)
        {
          QPoint position((*alien)->Box().CentralPoint());
          QSize size((*alien)->Box().Size());
          m_explosions.push_back({position, size, 0, m_explosionFrames.Pause});
          m_aliens.erase(alien);
        }

        hit = true;
        break;
      }
      alien++;
    }
    if (!hit) obstacle++;
  }
}

void Space::DrawAliens(TexturedRect * texturedRect)
{
  Logger::Instance() << "draw_aliens\n";
  for (auto it = m_aliens.begin(); it != m_aliens.end(); ++it)
  {
    QVector2D position((*it)->Box().CentralPoint().x(),
       m_screenSize.height() - (*it)->Box().CentralPoint().y());
    texturedRect->Render(beingConfigs[(*it)->TypeBeing()].texture, position, (*it)->Box().Size(), m_screenSize, 0);
  }
}

void Space::CheckCreateItem()
{
  Logger::Instance() << "check_create_item\n";
  if (Time() % 1000 == 0 && Time() >= 1000 && Probability(90) ||
      Time() % 200 == 0 && Time() >= 200 && Probability(10))
  {
    int typeItem = rand() % 3;
    int width = itemConfigs[typeItem].width;
    int height = itemConfigs[typeItem].height;
    Point2D position(rand() % (m_screenSize.width() - 2 * width) + width, -height * 2);
    m_items.push_back(Factory::Create<Item>(position, Point2D(0, 1), typeItem));
  }
}

void Space::MoveItems()
{
  Logger::Instance() << "move_items\n";
  for (auto it = m_items.begin(); it != m_items.end();)
  {
    Point2D centralPoint = Point2D((*it)->Box().CentralPoint());
    int speed = itemConfigs[(*it)->TypeItem()].speed;
    (*it)->Move(centralPoint + (*it)->Direction() * speed);
    if (centralPoint.y() >= m_screenSize.height() + (*it)->Box().PointMax().y())
    {
      it = m_items.erase(it);
    } else ++it;
  }
}

void Space::CheckCollideItems()
{
  Logger::Instance() << "check_collide_items\n";
  for (auto it = m_items.begin(); it != m_items.end();)
  {
    if ((*it)->Box().IntersectBox(m_player.Box()))
    {
      int typeItem = (*it)->TypeItem();
      switch (typeItem)
      {
      case 0:
        m_player.SetHp(m_player.Hp() + 50);
        if (m_player.Hp() > m_player.MaxHp) m_player.SetHp(m_player.MaxHp);
        break;
      case 1:
        m_player.SetGun(1);
        m_player.SetAmmo(150);
        break;
      case 2:
        m_player.SetGun(5);
        m_player.SetAmmo(30);
        break;
      }

      it = m_items.erase(it);
      continue;
    }
    it++;
  }
}

void Space::DrawItems(TexturedRect * texturedRect)
{
  Logger::Instance() << "draw_items\n";
  for (auto it = m_items.begin(); it != m_items.end(); ++it)
  {
    QVector2D position((*it)->Box().CentralPoint().x(),
       m_screenSize.height() - (*it)->Box().CentralPoint().y());
    texturedRect->Render(itemConfigs[(*it)->TypeItem()].texture, position, (*it)->Box().Size(), m_screenSize, 0);
    }
}
