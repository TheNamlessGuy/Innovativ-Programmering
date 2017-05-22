#ifndef _WALL_H_
#define _WALL_H_
#include "console.h"

class Wall
{
 private:
  int x{0}, y{0};
  std::string icon = "";
  bool horizontal{true};
 public:
  Wall();
  Wall(int x, int y, bool horizontal, int length);
  void render(Console& c);
};
#endif
