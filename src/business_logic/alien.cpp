#include "Alien.h"
#include "geometry/Point2D.h"
#include "Global_configs.h"
#include "geometry/Box2D.h"
#include "patterns/Logger.h"
#include <cmath>

Alien::Alien(Box2D const & box, Point2D const & direction, int const typeBeing) :
  GameBeing(box, direction, typeBeing)
{
  Logger::Instance() << "Constructor " << *this << "\n";
  ChooseBehavior();
  SetSpeed(beingConfigs[typeBeing].maxSpeed);
}

Alien::Alien(Point2D const & position, Point2D const & direction, int const typeBeing) :
  GameBeing(position, direction, typeBeing)
{
  Logger::Instance() << "Constructor " << *this << "\n";
  ChooseBehavior();
  SetSpeed(beingConfigs[typeBeing].maxSpeed);
}

void Alien::SetOnHit(Alien::TOnHit * const onHit)
{
  m_onHit = onHit;
}

void Alien::Hit(int const typeBullet)
{
//  if (m_onHit != nullptr && (*m_onHit) != nullptr)
//  {
//    (*m_onHit)(typeBullet);
//  }
  SetHp(Hp() - bulletConfigs[typeBullet].damage);
  Logger::Instance() << "HP = " << Hp() << "\n";
}

void Alien::SetBehavior(TBehavior * behavior)
{
  m_behavior = behavior;
}

void Alien::Behavior(Space & space)
{
  if (m_behavior != nullptr && (*m_behavior) != nullptr) (*m_behavior)(space);
}

void Alien::ChooseBehavior()
{
  TBehavior * behavior = new TBehavior;
  switch (TypeBeing())
  {
    case 1:
      *behavior = ([this] (Space & space)
      {
        Point2D centralPoint(this->Box().PCentralPoint());
        double speed = beingConfigs[this->TypeBeing()].speed0;
        Point2D newPoint(centralPoint + this->Direction() * speed);
        this->Move(newPoint);

        if (this->CallGun().Pause() > 0) this->CallGun().SetPause(this->CallGun().Pause() - 10);
        if (this->CallGun().Pause() <= 0)
        {
          this->CallGun().SetPause(gunConfigs[this->CallGun().TypeGun()].delay);
          Point2D shotPoint(this->Box().PCentralPoint() + this->Direction() * this->Box().Height() / 2
                            - this->Direction() * 10);
          this->CallGun().Shot(shotPoint, Point2D(0, 1), space.m_bullets);
        }
      });
      break;
    case 2:
      *behavior = ([this] (Space & space)
      {
        Point2D centralPoint(this->Box().PCentralPoint());
        double speed = beingConfigs[this->TypeBeing()].speed0;
        Point2D newPoint(centralPoint + this->Direction() * speed);
        this->Move(newPoint);

        this->CallGun().pause -= 10;
        if (this->CallGun().bulletsLeft > 0 && this->CallGun().pause <= 0)
        {
          this->CallGun().pause = 50;
          this->CallGun().bulletsLeft--;
          Point2D shotPoint(this->Box().PCentralPoint() + this->Direction() * this->Box().Height() / 2
                            - this->Direction() * 10);
          this->CallGun().Shot(shotPoint, Point2D(0, 1), space.m_bullets);
        }
        if (this->CallGun().Pause() > 0) this->CallGun().SetPause(this->CallGun().Pause() - 10);
        if (this->CallGun().Pause() <= 0 &&
            abs(space.m_player.Box().CentralPoint().x() - this->Box().CentralPoint().x()) < 80)
        {
          this->CallGun().SetPause(gunConfigs[this->CallGun().TypeGun()].delay);
          Point2D shotPoint(this->Box().PCentralPoint() + this->Direction() * this->Box().Height() / 2
                            - this->Direction() * 10);
          this->CallGun().Shot(shotPoint, Point2D(0, 1), space.m_bullets);
          this->CallGun().bulletsLeft = 2;
          this->CallGun().pause = 50;
        }
      });
      break;
    case 3:
      *behavior = ([this] (Space & space)
      {
        Point2D centralPoint(this->Box().PCentralPoint());
        double speedY = beingConfigs[this->TypeBeing()].speed0;
        if (this->Speed() > 0 && this->Box().PointMax().x() >= space.m_screenSize.width() ||
            this->Speed() < 0 && this->Box().PointMin().x() < 0) this->SetSpeed(-this->Speed());
        double speedX = this->Speed();
        Point2D newPoint(centralPoint + Point2D(speedX, speedY));
        this->Move(newPoint);

        this->CallGun().pause -= 10;
        if (this->CallGun().bulletsLeft > 0 && this->CallGun().pause <= 0)
        {
          this->CallGun().pause = 50;
          this->CallGun().bulletsLeft--;
          Point2D shotPoint(this->Box().PCentralPoint() + this->Direction() * this->Box().Height() / 3
                            - this->Direction() * 10);
         // this->CallGun().SetTypeGun(6);
         // this->CallGun().Shot(shotPoint + Point2D(this->Box().Width() / 4, 0), Point2D(0, 1), space.m_bullets);
         // this->CallGun().Shot(shotPoint - Point2D(this->Box().Width() / 4, 0), Point2D(0, 1), space.m_bullets);
         // this->CallGun().SetTypeGun(4);
        }

        if (this->CallGun().Pause() > 0) this->CallGun().SetPause(this->CallGun().Pause() - 10);
        if (this->CallGun().Pause() <= 0)
        {
          this->CallGun().SetPause(gunConfigs[this->CallGun().TypeGun()].delay);
          Point2D shotPoint(this->Box().PCentralPoint() + this->Direction() * this->Box().Height() / 2
                            - this->Direction() * 10);
          Point2D direction(space.m_player.Box().PCentralPoint() - shotPoint);
          direction.Normalize();
          this->CallGun().Shot(shotPoint, direction, space.m_bullets);
          this->CallGun().bulletsLeft = 4;
          this->CallGun().pause = 50;
        }
      });
      break;
    }
    SetBehavior(behavior);
}
