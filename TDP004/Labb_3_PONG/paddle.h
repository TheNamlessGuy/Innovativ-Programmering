#ifndef _PADDLE_H_
#define _PADDLE_H_
#include "console.h"

class Paddle
{
 private:
  float x{0}, y{0}, old_x{0}, old_y{0};
  float speed{0};
  std::string icon{"o--------------------o"};
 public:
  Paddle();
  Paddle(int in_x, int in_y);
  std::string get_icon();
  int get_icon_length();
  int get_x();
  int get_y();
  void move_left();
  void move_right();
  float get_speed();
  void set_speed(float in_speed);
  void render(Console& c);
};
#endif
