#pragma once
#include "GameObject.h"
#include "Gun.h"
#include <QOpenGLTexture>
#include <memory>

class GameBeing : public GameObject
{
public:
  GameBeing(Box2D const & box, Point2D const & direction, int const typeBeing);
  GameBeing(Point2D const & position, Point2D const & direction, int const typeBeing);
  ~GameBeing();
  int const & TypeBeing() const;
  int const Hp() const;
  void SetHp(int const & val);
  double const Speed() const;
  void SetSpeed(double const val);
  Gun & CallGun();
  QOpenGLTexture * m_texture = nullptr;
  void SetGun(int const typeGun);
private:
  int m_typeBeing;
  Gun m_gun;
  int m_hp;
  double m_speed;
};
