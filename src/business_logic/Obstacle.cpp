#include "Obstacle.h"
#include "Global_configs.h"
#include "geometry/Point2D.h"

Obstacle::Obstacle(Point2D const & position, Point2D const & direction, int const width, int const typeObstacle) :
  m_typeObstacle(typeObstacle), m_hp(obstacleConfigs[typeObstacle].hp), m_inside(false)
{
  m_deltaAngle = (double)(rand() % 8 + 2) / 10.0 * M_PI / 180.0 * (rand() % 2 == 0 ? 1.0 : -1.0);
  m_speed = (double)(3 + rand() % (12 - 3)) / 10.0;
  Point2D deltaPoint(width / 2, width / 2);
  Box2D box(position - deltaPoint, position + deltaPoint);
  SetParameters(box, direction);
  Logger::Instance() << "obstacle created " << this->Box() << " " << this->Direction() << "\n";
}

int const Obstacle::Hp() const
{
  return m_hp;
}
void Obstacle::SetHp(int const & val)
{
  m_hp = val;
}

bool const Obstacle::Inside()
{
  return m_inside;
}

void Obstacle::SetInside(bool const val)
{
  m_inside = val;
}

int const Obstacle::TypeObstacle()
{
  return m_typeObstacle;
}

void Obstacle::Rotate()
{
  m_angle += m_deltaAngle;
  if (m_angle >= 2 * M_PI)
  {
    m_angle -= 2 * M_PI;
  } else
  if (m_angle <= 2 * M_PI)
  {
    m_angle += 2 * M_PI;
  }
}
