#include "Alien.h"
#include "geometry/Point2D.h"
#include "Global_configs.h"
#include "geometry/Box2D.h"
#include "patterns/Logger.h"

Alien::Alien(Box2D const & box, Point2D const & direction, int const typeBeing) :
  GameBeing(box, direction, typeBeing)
{
  Logger::Instance() << "Constructor " << *this << "\n";
}

void Alien::SetOnHit(Alien::TOnHit * const onHit)
{
  m_onHit = onHit;
}

void Alien::Hit(int const typeBullet)
{
  if (m_onHit != nullptr && (*m_onHit) != nullptr)
  {
    (*m_onHit)(typeBullet);
  }
  SetHp(Hp() - bulletConfigs[typeBullet].damage);
}
