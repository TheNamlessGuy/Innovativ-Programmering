#include "ball.h"
#include "console.h"
Ball::Ball()
{
  x = 0;
  y = 0;
}
Ball::Ball(int in_x, int in_y)
{
  x = in_x;
  y = in_y;
  old_x = x;
  old_y = y;
}
int Ball::get_x()
{
  return int(x);
}
int Ball::get_y()
{
  return int(y);
}
void Ball::change_pos(float in_x, float in_y)
{
  old_x = x;
  old_y = y;
  x = in_x;
  y = in_y;
}
float Ball::get_x_speed()
{
  return x_speed;
}
float Ball::get_y_speed()
{
  return y_speed;
}
void Ball::set_speed(float in_x_speed, float in_y_speed)
{
  x_speed = in_x_speed;
  y_speed = in_y_speed;
}
char Ball::get_icon()
{
  return icon;
}
void Ball::set_icon(char c)
{
  icon = c;
}
void Ball::render(Console& c)
{
  c.setPos(old_x, old_y);
  c.put(' ');
  c.setPos(x, y);
  c.put(icon);
}
