#include "level.h"
#include "ball.h"
#include "paddle.h"
#include "wall.h"
#include "console.h"
#include <vector>
using namespace std;

Level::Level(Ball b, Paddle p, vector<Wall> w, Wall g)
{
  ball = b;
  paddle = p;
  walls = w;
  goal = g;
}
void Level::update(char c, bool& game_over)
{
  switch(c)
    {
    case 'a':
      break;
    case 'd':
      break;
    case 'q':
      game_over = true;
      break;
    default:
      break;
    }
}
void Level::render(Console& c)
{
  ball.render(c);
  paddle.render(c);
  render_walls(c);
}
void Level::render_walls(Console& c)
{
  for (unsigned int i = 0; i < walls.size(); ++i)
    {
      walls[i].render(c);
    }
  goal.render(c);
}
