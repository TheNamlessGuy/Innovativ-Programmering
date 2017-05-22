#include "aabb.h"

#include <iostream>
#include <algorithm>

using namespace std;

AABB::AABB(double top_var, double left_var,
           double bottom_var, double right_var):
  top(top_var), left(left_var), bottom(bottom_var), right(right_var)
{
  if (top > bottom)
  {
    swap(top, bottom);
  }
  if (left > right)
  {
    swap(left, right);
  }
}

bool AABB::contain(double x, double y)
{
  bool top_bottom{(y >= top && y <= bottom)};
  bool left_right{(x >= left && x <= right)};
  return (top_bottom && left_right);
}

bool AABB::contain(Point pt)
{
  return contain(double(pt.x), double(pt.y));
}

bool AABB::intersect(AABB a)
{
  double a_top{round(a.get_top())};
  double a_bottom{round(a.get_bottom())};
  double a_left{round(a.get_left())};
  double a_right{round(a.get_right())};
  
  if (right < a_left || left > a_right)
  {
    return false;
  }
  if (bottom < a_top || top > a_bottom)
  {
    return false;
  }
  return true;
}

AABB AABB::min_bounding_box(AABB a)
{
  AABB return_box(min(top, a.get_top()), min(left, a.get_left()),
                  max(bottom, a.get_bottom()), max(right, a.get_right()));
  return return_box;
}

bool AABB::may_collide(AABB from, AABB to)
{
  AABB full_path{from.min_bounding_box(to)};
  return (intersect(full_path));
}

bool AABB::collision_point(AABB from, AABB to, Point& where)
{
  if (may_collide(from, to))
  {
    return collision_check(from, to, where);
  }
  return false;
}

bool AABB::collision_check(AABB from, AABB to, Point& where)
{
  double x_movement{to.get_left() - from.get_left()};
  double y_movement{to.get_top() - from.get_top()};
  double x_move;
  double y_move;
  double max_value;
  
  if (max(fabs(x_movement), fabs(y_movement)) == fabs(x_movement))
  {
    max_value = fabs(x_movement);
    y_move = y_movement / fabs(x_movement);
    x_move = x_movement / fabs(x_movement);
  }
  else
  {
    max_value = fabs(y_movement);
    x_move = x_movement / fabs(y_movement);
    y_move = y_movement / fabs(y_movement);
  }
  
  for (int i = 0; i < int(max_value); ++i)
  {
    from.move_x(x_move);
    from.move_y(y_move);
    if (intersect(from))
    {
      where.x = int(from.get_left() - x_move);
      where.y = int(from.get_top() - y_move);
      return true;
    }  
  }
  
  return false;
}

double AABB::get_top()
{
  return top;
}

double AABB::get_bottom()
{
  return bottom;
}

double AABB::get_left()
{
  return left;
}

double AABB::get_right()
{
  return right;
}
void AABB::move_x(double amount)
{
  left += amount;
  right += amount;
}
void AABB::move_y(double amount)
{
  top += amount;
  bottom += amount;
}
