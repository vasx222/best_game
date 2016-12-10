#pragma once
#include "GameObject.h"
#include <QMessageBox>

class Bullet : public GameObject
{
public:
  Bullet() = default;
  Bullet(Box2D const & box, Point2D const & direction, int const typeBullet, int const owner);
  Bullet(Point2D const & position, Point2D const & direction, int const typeBullet, int const owner);
  int const & TypeBullet() const;
  int const & Owner() const;
  ~Bullet() {Logger::Instance() << "Bullet destroyed\n";}
private:
  int m_typeBullet;
  int m_owner;
};
