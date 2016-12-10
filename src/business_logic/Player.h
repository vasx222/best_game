#pragma once
#include "GameObject.h"
#include "Gun.h"
#include "GameBeing.h"
#include <qopengltexture.h>

class Player : public GameBeing
{
public:
  Player(Box2D const & box, Point2D const & direction, int const typeBeing);
  Player(Point2D const & position, Point2D const & direction, int const typeBeing);
  void SetAmmo(int const ammo) {m_ammo = ammo;}
  int const Ammo() {return m_ammo;}
  int const MaxHp = 500;
private:
  int m_ammo = -1;

};
