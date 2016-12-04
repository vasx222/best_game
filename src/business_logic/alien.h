#pragma once
#include "Gun.h"
#include "GameObject.h"
#include <functional>
#include "Global_configs.h"
#include "patterns/logger.h"
#include <fstream>
#include "gamebeing.h"
#include <QOpenGLTexture>

class Alien : public GameBeing
{
public:
  using TOnHit = std::function<void(int const)>;
  Alien(Box2D const & box, Point2D const & direction, int const typeBeing);
  void SetOnHit(TOnHit * const onHit);
  void Hit(int const typeBullet);
private:
  TOnHit * m_onHit;
};
