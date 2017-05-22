#ifndef _BALL_H_
#define _BALL_H_
#include "console.h"
class Ball
{
 private:
  float x{0}, y{0}, old_x{0}, old_y{0};
  float x_speed{0}, y_speed{0};
  char icon{'o'};
 public:
  Ball();
  Ball(int in_x, int in_y);
  int get_x();
  int get_y();
  void change_pos(float in_x, float in_y);
  float get_x_speed();
  float get_y_speed();
  void set_speed(float in_x_speed, float in_y_speed);
  char get_icon();
  void set_icon(char c);
  void render(Console& c);
};
#endif
