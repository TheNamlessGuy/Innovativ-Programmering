#ifndef _LEVEL_H_
#define _LEVEL_H_
#include "console.h"
#include "ball.h"
#include "wall.h"
#include "paddle.h"
#include <vector>

class Level
{
 private:
  Ball ball;
  Paddle paddle;
  Wall goal;
  std::vector<Wall> walls;
  void render_walls(Console& c);
 public:
  Level(Ball b, Paddle p, std::vector<Wall> w, Wall g);
  void update(char c, bool& game_over);
  void render(Console& c);
};
#endif
