#include "Player.h"
#include "Global_configs.h"

Player::Player(Box2D const & box, Point2D const & direction, int const typeBeing) :
  GameBeing(box, direction, typeBeing), m_ammo(beingConfigs[typeBeing].ammo)
{
  Logger::Instance() << "Constructor " << *this << "\n";
}

Player::Player(Point2D const & position, Point2D const & direction, int const typeBeing) :
  GameBeing(position, direction, typeBeing)
{
  Logger::Instance() << "Constructor " << *this << "\n";
}
