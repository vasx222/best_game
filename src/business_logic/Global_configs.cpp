#include "Global_configs.h"
#include <cmath>

std::vector <TBullet> bulletConfigs;
std::vector <TGun> gunConfigs;
std::vector <TBeing> beingConfigs;
std::vector <TObstacle> obstacleConfigs;
std::vector <TItem> itemConfigs;

void InitConfig()
{
  bulletConfigs.clear(); gunConfigs.clear(); beingConfigs.clear(); obstacleConfigs.clear(); itemConfigs.clear();

  //Obstacle 0
  TObstacle obstacle0; obstacle0.texture = nullptr; obstacle0.fileName = "data/ready/asteroid1.png";
  obstacle0.hp = 1000; obstacle0.deltaAngle = 1 * M_PI / 180; obstacle0.minWidth = 50; obstacle0.maxWidth = 120;
  obstacle0.speed = 1; obstacle0.damage = 20; obstacleConfigs.push_back(obstacle0);

  //Obstacle 1
  TObstacle obstacle1; obstacle1.texture = nullptr; obstacle1.fileName = "data/ready/asteroid2.png";
  obstacle1.hp = 1000; obstacle1.deltaAngle = 1 * M_PI / 180; obstacle1.minWidth = 50; obstacle1.maxWidth = 120;
  obstacle1.speed = 1; obstacle1.damage = 20; obstacleConfigs.push_back(obstacle1);

  //player standart bullet
  TBullet bullet0; bullet0.damage = 10; bullet0.damageRadius = 1; bullet0.speed = 10; bullet0.width = 15;
  bullet0.height = 16; bullet0.color = QColor(200, 0, 0); bullet0.texture = nullptr;
  bullet0.fileName = "data/ready/red_bullet.png"; bulletConfigs.push_back(bullet0);

  //laser bullet
  TBullet bullet1; bullet1.damage = 10; bullet1.damageRadius = 1; bullet1.speed = 30; bullet1.width = 5;
  bullet1.height = 35; bullet1.color = QColor(66, 255, 114); bullet1.texture = nullptr;
  bullet1.fileName = "data/ready/green_bullet.png"; bulletConfigs.push_back(bullet1);

  //alien1 bullet
  TBullet bullet2; bullet2.damage = 10; bullet2.damageRadius = 1; bullet2.speed = 7; bullet2.width = 13;
  bullet2.height = 16; bullet2.color = QColor(200, 0, 0); bullet2.texture = nullptr;
  bullet2.fileName = "data/ready/red_bullet_2.png"; bulletConfigs.push_back(bullet2);

  //alien2 bullet
  TBullet bullet3; bullet3.damage = 10; bullet3.damageRadius = 1; bullet3.speed = 12; bullet3.width = 20;
  bullet3.height = 20; bullet3.color = QColor(20, 20, 20); bullet3.texture = nullptr;
  bullet3.fileName = "data/ready/blue_double_bullet.png"; bulletConfigs.push_back(bullet3);

  //alien3 bullet central
  TBullet bullet4; bullet4.damage = 10; bullet4.damageRadius = 1; bullet4.speed = 12; bullet4.width = 14;
  bullet4.height = 25; bullet4.color = QColor(200, 200, 0); bullet4.texture = nullptr;
  bullet4.fileName = "data/ready/white_bullet.png"; bulletConfigs.push_back(bullet4);

  //alien3 bullet side
  TBullet bullet5; bullet5.damage = 10; bullet5.damageRadius = 1; bullet5.speed = 12; bullet5.width = 20;
  bullet5.height = 20; bullet5.color = QColor(200, 200, 0); bullet5.texture = nullptr;
  bullet5.fileName = "data/ready/green_bullet.png"; bulletConfigs.push_back(bullet5);

  //player bullet heavy_gun
  TBullet bullet6; bullet6.damage = 20; bullet6.damageRadius = 1; bullet6.speed = 12; bullet6.width = 25;
  bullet6.height = 25; bullet6.color = QColor(200, 200, 0); bullet6.texture = nullptr;
  bullet6.fileName = "data/ready/red_bullet.png"; bulletConfigs.push_back(bullet6);

  //player standart gun
  TGun gun0; gun0.delay = 300; gun0.typeBullet = 0; gunConfigs.push_back(gun0);

  //laser gun
  TGun gun1; gun1.delay = 60; gun1.typeBullet = 1; gunConfigs.push_back(gun1);

  //alien1 gun
  TGun gun2; gun2.delay = 900; gun2.typeBullet = 2; gunConfigs.push_back(gun2);

  //alien2 gun
  TGun gun3; gun3.delay = 900; gun3.typeBullet = 3; gunConfigs.push_back(gun3);

  //alien3 gun 1
  TGun gun4; gun4.delay = 1000; gun4.typeBullet = 4/*5*/; gunConfigs.push_back(gun4);

  //player heavy_gun
  TGun gun5; gun5.delay = 400; gun5.typeBullet = 6; gunConfigs.push_back(gun5);

  //alien3 gun 2
  TGun gun6; gun6.delay = 500; gun6.typeBullet = 5; gunConfigs.push_back(gun6);

  //player
  TBeing being0; being0.hp = 500; being0.speed0 = 0; being0.acceleration = 0.1; being0.maxSpeed = 10;
  being0.width = 100; being0.height = 100; being0.typeGun = 0; being0.ammo = -1;
  being0.fileName = "data/ready/ship.png"; being0.texture = nullptr; beingConfigs.push_back(being0);

  //simple shooting alien
  TBeing being1; being1.hp = 40; being1.speed0 = 0.7; being1.acceleration = 0; being1.maxSpeed = 0.5;
  being1.width = 110; being1.height = 91; being1.typeGun = 2; being1.ammo = -1; being1.score = 1;
  being1.fileName = "data/ready/alien1.png"; being1.texture = nullptr; beingConfigs.push_back(being1);

  //shooting smart alien
  TBeing being2; being2.hp = 50; being2.speed0 = 0.5; being2.acceleration = 0; being2.maxSpeed = 0.5;
  being2.width = 110; being2.height = 93; being2.typeGun = 3; being2.ammo = -1; being2.score = 2;
  being2.fileName = "data/ready/alien2.png"; being2.texture = nullptr; beingConfigs.push_back(being2);

  //shooting horizontal alien
  TBeing being3; being3.hp = 1500; being3.speed0 = 0.2; being3.acceleration = 0; being3.maxSpeed = 1.2;
  being3.width = 250; being3.height = 175; being3.typeGun = 4; being3.ammo = -1; being3.score = 10;
  being3.fileName = "data/ready/alien3.png"; being3.texture = nullptr; beingConfigs.push_back(being3);

  TItem item0; item0.fileName = "data/ready/aid.png"; item0.texture = nullptr; item0.speed = 1.0;
  item0.width = 35; item0.height = 37; itemConfigs.push_back(item0);

  TItem item1; item1.fileName = "data/ready/laser.png"; item1.texture = nullptr; item1.speed = 1.0;
  item1.width = 57; item1.height = 30; itemConfigs.push_back(item1);

  TItem item2; item2.fileName = "data/ready/heavy_gun.png"; item2.texture = nullptr; item2.speed = 1.0;
  item2.width = 54; item2.height = 30; itemConfigs.push_back(item2);
}
