#include "gamebeing.h"
#include "Global_configs.h"

GameBeing::GameBeing(Box2D const & box, Point2D const & direction, int const typeBeing) :
  m_typeBeing(typeBeing)
{
  SetParameters(box, direction);
  Point2D point(box.Width() / 2, box.Width() / 2);
  m_gun = Gun(Box2D(point, point), direction, beingConfigs[m_typeBeing].typeGun);
  m_hp = beingConfigs[m_typeBeing].hp;
  Logger::Instance() << "Constructor " << *this << "\n";
}

GameBeing::GameBeing(GameBeing const & obj)
{
  SetParameters(obj.Box(), obj.Direction());
  m_typeBeing = obj.TypeBeing();
  m_hp = beingConfigs[m_typeBeing].hp;
  Logger::Instance() << "Copy constructor " << *this << "\n";
}

GameBeing GameBeing::operator = (GameBeing const & obj)
{
  SetParameters(obj.Box(), obj.Direction());
  m_typeBeing = obj.TypeBeing();
  m_hp = beingConfigs[m_typeBeing].hp;
  Logger::Instance() << "Copy operator " << *this << "\n";
  return *this;
}

GameBeing::~GameBeing()
{
  delete m_texture;
}

int const & GameBeing::TypeBeing() const
{
  return m_typeBeing;
}

int const & GameBeing::Hp() const
{
  return m_hp;
}

void GameBeing::SetHp(int const & val)
{
  m_hp -= val;
}
