#include "Item.h"
#include "business_logic/Global_configs.h"

Item::Item(Point2D const & position, Point2D const & direction, int const typeItem) :
  m_typeItem(typeItem)
{
  int width = itemConfigs[typeItem].width;
  int height = itemConfigs[typeItem].height;
  Point2D deltaPoint(width / 2, height / 2);
  Box2D box(position - deltaPoint, position + deltaPoint);
  SetParameters(box, direction);
  Logger::Instance() << "Item created\n";
}
