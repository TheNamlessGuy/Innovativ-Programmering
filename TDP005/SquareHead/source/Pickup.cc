#include <SDL2/SDL.h>
#include <random>
#include <iostream>

#include "../headers/Pickup.h"
#include "../headers/Image.h"
#include "../headers/CollisionBox.h"

using namespace std;

Pickup::Pickup(int const& x_i, int const& y_i, SDL_Renderer* const& r)
	: img(x_i, y_i, "src/images/pickup.png", r),
	  x(x_i), y(y_i), lifespan(500) {}

Pickup::~Pickup() {}

void Pickup::render(SDL_Renderer* const& r) const
{
	img.render(r);
}

bool Pickup::update()
{
	lifespan--;

	return (lifespan <= 0);
}

bool Pickup::is_colliding_with(SDL_Rect const& player)
{
	return (is_in(get_hitbox(), player));
}

pair<bool, int> Pickup::pick_up(long long const& score) const
{
	random_device rd;
	uniform_int_distribution<int> uid(0, 99);

	pair<bool, int> return_pair;

	return_pair.second = uid(rd);

	if (return_pair.second < 20)
	{
		//WEAPON
		return_pair.first = true;
		return_pair.second %= 4;
	}
	else if (return_pair.second < 40)
	{
		//POWERUP
		return_pair.first = false;
		if (score < -3000)
		{
			return_pair.second %= 5;
		}
		else
		{
			return_pair.second %= 3;
		}
		return_pair.second += 1;
	}
	else
	{
		//ARMOR
		return_pair.first = false;
		return_pair.second = 0;
	}

	return return_pair;
}

SDL_Rect Pickup::get_hitbox()
{
	return img.get_hitbox();
}
