#include <vector>
#include <iostream>

#include "../headers/Grenade.h"

using namespace std;

Grenade::Grenade(int const& x_i, int const& y_i,
		int const& x_speed_i, int const& y_speed_i,
		bool const& friendly_i)
	: Bullet(x_i, y_i, x_speed_i, y_speed_i, friendly_i), fuse_timer(3)
{
	r = 0;
	g = 150;
	b = 0;
}

Grenade::~Grenade()
{}

vector<Bullet*> Grenade::update()
{
	fuse_timer -= 0.1;

	vector<Bullet*> bullets;

	x += x_speed;
	if (x > 800 || x < 0)
	{
		gone = true;
	}

	y += y_speed;
	if (y > 600 || y < 0)
	{
		gone = true;
	}

	if (fuse_timer <= 0)
	{
		gone = true;

		//Starts at north, goes clockwise
		bullets.push_back(new Bullet(x, y, 0, -SPEED, friendly));
		bullets.push_back(new Bullet(x, y, SPEED, -SPEED, friendly));
		bullets.push_back(new Bullet(x, y, SPEED, 0, friendly));
		bullets.push_back(new Bullet(x, y, SPEED, SPEED, friendly));
		bullets.push_back(new Bullet(x, y, 0, SPEED, friendly));
		bullets.push_back(new Bullet(x, y, -SPEED, SPEED, friendly));
		bullets.push_back(new Bullet(x, y, -SPEED, 0, friendly));
		bullets.push_back(new Bullet(x, y, -SPEED, -SPEED, friendly));
	}

	return bullets;
}
