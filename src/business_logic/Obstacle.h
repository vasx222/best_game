#pragma once
#include "GameObject.h"
#include "patterns/logger.h"
#include <QMessageBox>
#include "Global_configs.h"

class Obstacle : public GameObject
{
public:
  Obstacle() = default;
  Obstacle(Point2D const & position, Point2D const & direction, int const width, int const typeObstacle);
  ~Obstacle() {Logger::Instance() << "obstacle destroyed\n"; /*QMessageBox b; b.setText("!"); b.exec();*/}
  int const Hp() const;
  void SetHp(int const & val);
  bool const Inside();
  void SetInside(bool const val);
  int const TypeObstacle();
  void Rotate();
  double Angle() {return m_angle;}
  void Hit(int const typeBullet) {SetHp(Hp() - bulletConfigs[typeBullet].damage);}
  double Speed() {return m_speed;}
  void SetSpeed(double const speed) {m_speed = speed;}
private:
  int m_hp;
  bool m_inside = false;
  int m_typeObstacle;
  double m_angle = 0;
  double m_deltaAngle;
  double m_speed;
};
