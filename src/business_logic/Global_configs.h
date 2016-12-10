#pragma once
#include <vector>
#include "GameEntity.h"
#include <QString>
#include <QOpenGLTexture>
#include <QColor>

class TBullet
{
public:
  int damage;
  int speed;
  int width;
  int height;
  int damageRadius;
  QColor color;
  QOpenGLTexture * texture;
  QString fileName;
};
class TGun
{
public:
  int delay;
  int typeBullet;
};
class TBeing
{
public:
  int hp;
  double speed0;
  double acceleration;
  double maxSpeed;
  int width;
  int height;
  int typeGun;
  int ammo;
  QString fileName;
  QOpenGLTexture * texture;
  int score;
};

class TObstacle
{
public:
  QOpenGLTexture * texture;
  QString fileName;
  int minWidth;
  int maxWidth;
  double deltaAngle;
  int hp;
  double speed;
  int damage;
};

class TItem
{
public:
  QOpenGLTexture * texture;
  QString fileName;
  int width;
  int height;
  double speed;
};

void InitConfig();
extern std::vector <TBullet> bulletConfigs;
extern std::vector <TGun> gunConfigs;
extern std::vector <TBeing> beingConfigs;
extern std::vector <TObstacle> obstacleConfigs;
extern std::vector <TItem> itemConfigs;


