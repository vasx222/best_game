#pragma once
#include "Gun.h"
#include "GameObject.h"
#include <functional>
#include "Global_configs.h"
#include "patterns/logger.h"
#include <fstream>
#include "gamebeing.h"
#include <QOpenGLTexture>
#include "business_logic/Space.h"

class Space;

class Alien : public GameBeing
{
public:
  using TOnHit = std::function<void(int const)>;
  using TBehavior = std::function<void(Space &)>;
  Alien(Box2D const & box, Point2D const & direction, int const typeBeing);
  Alien(Point2D const & position, Point2D const & direction, int const typeBeing);
  void SetOnHit(TOnHit * const onHit);
  void Hit(int const typeBullet);
  ~Alien() {Logger::Instance() << "Alien destroyed\n"; delete m_behavior;}
  void SetBehavior(TBehavior * behavior);
  void Behavior(Space & space);
  void ChooseBehavior();
private:
  TOnHit * m_onHit = nullptr;
  TBehavior * m_behavior = nullptr;
};
