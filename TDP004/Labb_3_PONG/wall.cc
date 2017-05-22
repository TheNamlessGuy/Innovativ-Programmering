#include "wall.h"
#include "console.h"
using namespace std;

Wall::Wall()
{
  x = 0;
  y = 0;
}
Wall::Wall(int x, int y, bool horizontal, int length)
{
  this->x = x;
  this->y = y;
  this->horizontal = horizontal;
  if (!horizontal)
    {
      icon = '+' + string(length - 2, '|') + '+';
    }
  else
    {
      icon = '+' + string(length -2, '-') + '+';
    }
}
void Wall::render(Console& c)
{
  c.setPos(x, y);
  if (horizontal)
    {
      c.put(icon);
    }
  else
    {
      for (unsigned int i = 0; i < icon.length(); ++i)
	{
	  c.setPos(x, (y + i));
	  c.put(icon.at(i));
	}
    }
}
