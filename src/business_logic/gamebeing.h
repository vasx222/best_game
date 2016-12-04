#pragma once
#include "GameObject.h"
#include "Gun.h"
#include <QOpenGLTexture>

class GameBeing : public GameObject
{
public:
  GameBeing(Box2D const & box, Point2D const & direction, int const typeBeing);
  GameBeing(GameBeing const & obj);
  GameBeing operator = (GameBeing const & obj);
  ~GameBeing();
  int const & TypeBeing() const;
  int const & Hp() const;
  void SetHp(int const & val);

  QOpenGLTexture * m_texture = nullptr;
private:
  int m_typeBeing;
  Gun m_gun;
  int m_hp;
};
