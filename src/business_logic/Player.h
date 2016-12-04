#pragma once
#include "GameObject.h"
#include "Gun.h"
#include "GameBeing.h"
#include <qopengltexture.h>

class Player : public GameBeing
{
public:
  Player(Box2D const & box, Point2D const & direction, int const typeBeing);

private:
  int m_ammo;

};
