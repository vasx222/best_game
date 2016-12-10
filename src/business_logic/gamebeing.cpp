#include "gamebeing.h"
#include "Global_configs.h"

GameBeing::GameBeing(Box2D const & box, Point2D const & direction, int const typeBeing) :
  m_typeBeing(typeBeing)
{
  SetParameters(box, direction);
  m_gun = Gun(direction, beingConfigs[m_typeBeing].typeGun, typeBeing);
  m_hp = beingConfigs[m_typeBeing].hp;
  m_speed = beingConfigs[m_typeBeing].speed0;
  Logger::Instance() << "Constructor " << *this << "\n";
}

GameBeing::GameBeing(Point2D const & position, Point2D const & direction, int const typeBeing) :
  m_typeBeing(typeBeing)
{
  int width = beingConfigs[m_typeBeing].width;
  int height = beingConfigs[m_typeBeing].height;
  Point2D deltaPoint(width / 2, height / 2);
  Box2D box(position - deltaPoint, position + deltaPoint);
  SetParameters(box, direction);
  m_gun = Gun(direction, beingConfigs[m_typeBeing].typeGun, typeBeing);
  m_hp = beingConfigs[m_typeBeing].hp;
  m_speed = beingConfigs[m_typeBeing].speed0;
  Logger::Instance() << "Constructor " << *this << "\n";
}

void GameBeing::SetGun(int const typeGun)
{
  m_gun.SetTypeGun(typeGun);
  //m_gun = Gun(Direction(), typeGun, TypeBeing());
}

GameBeing::~GameBeing()
{
  delete m_texture;
}

int const & GameBeing::TypeBeing() const
{
  return m_typeBeing;
}

int const GameBeing::Hp() const
{
  return m_hp;
}

void GameBeing::SetHp(int const & val)
{
  m_hp = val;
}

double const GameBeing::Speed() const
{
  return m_speed;
}

void GameBeing::SetSpeed(double const val)
{
  m_speed = val;
}

Gun & GameBeing::CallGun()
{
  return m_gun;
}
