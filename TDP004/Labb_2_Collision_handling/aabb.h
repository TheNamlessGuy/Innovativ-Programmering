#ifndef _AABB_H_
#define _AABB_H_

struct Point {
  int x;
  int y;
};

class AABB
{
public:
  
  AABB(double top_var, double left_var, double bottom_var, double right_var);
  bool contain(double x, double y);
  bool contain(Point pt);
  bool intersect(AABB a);
  AABB min_bounding_box(AABB a);
  bool may_collide(AABB from, AABB to);
  bool collision_point(AABB from, AABB to, Point& where);

private:
  
  double top;
  double left;
  double bottom;
  double right;
  
  bool collision_check(AABB from, AABB to, Point& where);
  double get_top();
  double get_bottom();
  double get_left();
  double get_right();
  void move_x(double amount);
  void move_y(double amount);
};
#endif
