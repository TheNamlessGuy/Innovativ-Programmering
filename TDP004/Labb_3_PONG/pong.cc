#include <iostream>
#include <vector>
#include "console.h"
#include "level.h"
#include "ball.h"
#include "paddle.h"
#include "wall.h"
using namespace std;

int main()
{
  Console console;
  Ball ball;
  Paddle paddle;
  Wall wall1(0, 0, true, console.getWidth());
  Wall wall2(0, 0, false, console.getHeight());
  Wall wall3(console.getWidth()-1, 0, false, console.getHeight());
  Wall goal(0, console.getHeight()-1, true, console.getWidth());
  vector<Wall> walls;
  walls.push_back(wall1);
  walls.push_back(wall2);
  walls.push_back(wall3);
  Level level(ball, paddle, walls, goal);
  char command{' '};
  bool game_over = false;
  while (!game_over)
    {
      if (console.get(command))
	{
	  level.update(command, game_over);
	}
      level.render(console);
    }
  return 0;
}
