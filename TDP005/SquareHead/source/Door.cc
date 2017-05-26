#include <SDL2/SDL.h>

#include "../headers/Door.h"
#include "../headers/Enemy.h"

using namespace std;

Door::Door(int const& x_i, int const& y_i, bool const& vertical, SDL_Renderer* r): x(x_i), y(y_i),
		is_vertical(vertical), a(DoorPart(r, vertical)), b(DoorPart(r, vertical)),
		is_opening(false), is_closing(false), amount_spawning(0)
{
	a.img->get_hitbox().x = x;
	a.img->get_hitbox().y = y;
	a.img->get_hitbox().w = 20;
	a.img->get_hitbox().h = 100;
	a.img->get_renderbox().w = 20;
	a.img->get_renderbox().h = 100;

	b.img->get_hitbox().x = x;
	b.img->get_hitbox().y = y + a.img->get_hitbox().h;
	b.img->get_hitbox().w = 20;
	b.img->get_hitbox().h = 100;
	b.img->get_renderbox().w = 20;
	b.img->get_renderbox().h = 100;

	a_orig = y;
	spawn_point = b.img->get_hitbox().y;

	if (!is_vertical)
	{
		swap(a.img->get_hitbox().h, a.img->get_hitbox().w);
		swap(a.img->get_renderbox().h, a.img->get_renderbox().w);
		swap(b.img->get_hitbox().h, b.img->get_hitbox().w);
		swap(b.img->get_renderbox().h, b.img->get_renderbox().w);
		b.img->get_hitbox().y = y;
		b.img->get_hitbox().x = x + a.img->get_hitbox().w;

		a_orig = x;
		spawn_point = b.img->get_hitbox().x;
	}
}

Door::~Door() {}

Door::DoorPart::DoorPart(SDL_Renderer* const& r, bool const& is_vertical)
	:img(new Image(0, 0, "src/images/doorpart.png", r))
{}

Door::DoorPart::~DoorPart()
{
	delete img;
}

void Door::render(SDL_Renderer* const& r) const
{
	a.img->render(r);
	b.img->render(r);
}

void Door::update()
{
	if (is_opening)
	{
		if (is_vertical)
		{
			if (a_orig - (a.img->get_hitbox().y + a.img->get_hitbox().h) >= 0)
			{
				is_opening = false;
			}
			else
			{
				a.img->get_hitbox().y -= movespeed;
				b.img->get_hitbox().y += movespeed;
			}
		}
		else
		{
			if (a_orig - (a.img->get_hitbox().x + a.img->get_hitbox().w) >= 0)
			{
				is_opening = false;
			}
			else
			{
				a.img->get_hitbox().x -= movespeed;
				b.img->get_hitbox().x += movespeed;
			}
		}
		return;
	}
	if (is_closing)
	{
		if (is_vertical)
		{
			if (a_orig <= a.img->get_hitbox().y)
			{
				is_closing = false;
			}
			else
			{
				a.img->get_hitbox().y += movespeed;
				b.img->get_hitbox().y -= movespeed;
			}
		}
		else
		{
			if (a_orig <= a.img->get_hitbox().x)
			{
				is_closing = false;
			}
			else
			{
				a.img->get_hitbox().x += movespeed;
				b.img->get_hitbox().x -= movespeed;
			}
		}
		return;
	}
}

void Door::open()
{
	is_opening = true;
}

void Door::close()
{
	is_closing = true;
}

void Door::spawn_enemy(vector<Enemy*>& enemies, SDL_Renderer* const& r)
{
	open();
	if (is_vertical)
	{
		if (x == 0) //If is on left side
		{
			enemies.push_back(new Enemy(x-52, spawn_point - 26, r));
		}
		else
		{
			enemies.push_back(new Enemy(x, spawn_point - 26, r));
		}
	}
	else
	{
		enemies.push_back(new Enemy(spawn_point - 26, y - 52, r));
	}
	close();
}

void Door::reset()
{
	is_opening = false;
	is_closing = false;
	if (is_vertical)
	{
		a.img->get_hitbox().y = a_orig;
		b.img->get_hitbox().y = a_orig + a.img->get_hitbox().h;
	}
	else
	{
		a.img->get_hitbox().x = a_orig;
		b.img->get_hitbox().x = a_orig + a.img->get_hitbox().w;
	}
}
