#pragma once
#include <memory>
#include "Bullet.h"
#include "GameObject.h"

class Gun : public GameObject
{
public:
  Gun() = default;
  Gun(Point2D const & direction, int const typeGun, int const owner);
  int const & TypeGun() const;
  int const & Owner() const;
  void Shot(Point2D const & position, Point2D const & direction,
            std::list<std::unique_ptr<Bullet> > & bullets);
  void SetPause(int const val);
  int const Pause() const;
  void SetTypeGun(int const typeGun);
  int bulletsLeft = 0;
  int pause = 0;
private:
  int m_typeGun;
  int m_owner;
  int m_pause = 0;
};
