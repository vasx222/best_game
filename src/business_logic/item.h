#pragma once
#include "GameObject.h"

class Item : public GameObject
{
public:
  Item() = default;
  Item(Point2D const & position, Point2D const & direction, int const typeItem);
  ~Item() {Logger::Instance() << "Item destroyed\n"; }
  int const TypeItem() {return m_typeItem;}
private:
  int m_typeItem;
};
