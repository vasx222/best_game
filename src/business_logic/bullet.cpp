#include "Bullet.h"
#include "Global_configs.h"
using namespace std;

Bullet::Bullet(Box2D const & box, Point2D const & direction, int const typeBullet, int const owner):
  m_typeBullet(typeBullet), m_owner(owner)
{
  SetParameters(box, direction);
  Logger::Instance() << "Constructor " << *this << "\n";
}

Bullet::Bullet(Point2D const & position, Point2D const & direction, int const typeBullet, int const owner) :
  m_typeBullet(typeBullet), m_owner(owner)
{
  int width = bulletConfigs[m_typeBullet].width;
  int height = bulletConfigs[m_typeBullet].height;
  Point2D deltaPoint(width / 2, height / 2);
  Box2D box(position - deltaPoint, position + deltaPoint);
  SetParameters(box, direction);
  Logger::Instance() << "Constructor " << *this << "\n";
}

int const & Bullet::TypeBullet() const
{
  return m_typeBullet;
}

int const & Bullet::Owner() const
{
  return m_owner;
}
