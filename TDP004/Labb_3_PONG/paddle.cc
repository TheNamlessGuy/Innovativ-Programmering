#include "paddle.h"
#include "console.h"
using namespace std;

Paddle::Paddle()
{
  x = 0;
  y = 0;
}
Paddle::Paddle(int in_x, int in_y)
{
  x = in_x;
  y = in_y;
  old_x = x;
  old_y = y;
}
std::string Paddle::get_icon()
{
  return icon;
}
int Paddle::get_icon_length()
{
  return icon.length();
}
int Paddle::get_x()
{
  return x;
}
int Paddle::get_y()
{
  return int(y);
}
void Paddle::move_left()
{
  old_x = x;
  x -= speed;
}
void Paddle::move_right()
{
  old_x = x;
  x += speed;
}
float Paddle::get_speed()
{
  return speed;
}
void Paddle::set_speed(float in_speed)
{
  speed = in_speed;
}
void Paddle::render(Console& c)
{
  //Fix so if not move no change
  c.setPos(old_x, old_y);
  c.put(string(icon.length(), ' '));
  c.setPos(x, y);
  c.put(icon);
}
