#include "Gun.h"
#include "Global_configs.h"
#include "patterns/factory.h"
#include "bullet.h"

Gun::Gun(Point2D const & direction, int const typeGun, int const owner) :
  m_typeGun(typeGun), m_owner(owner)
{
  SetParameters(Box2D(0, 0, 0, 0), direction);
  Logger::Instance() << "Constructor " << *this << "\n";
}

int const & Gun::TypeGun() const
{
  return m_typeGun;
}

int const & Gun::Owner() const
{
  return m_owner;
}

void Gun::Shot(Point2D const & position, Point2D const & direction,
               std::list<std::unique_ptr<Bullet> > & bullets)
{
  bullets.push_back(Factory::Create<Bullet>(position, direction,
                                            gunConfigs[m_typeGun].typeBullet, m_owner));
}

void Gun::SetPause(int const val)
{
  m_pause = val;
}

int const Gun::Pause() const
{
  return m_pause;
}

void Gun::SetTypeGun(int const typeGun)
{
  m_typeGun = typeGun;
}
