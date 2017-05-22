#include <SDL2/SDL.h>

#include "../headers/CollisionBox.h"

CollisionBox::CollisionBox() {}

CollisionBox::~CollisionBox() {}

bool CollisionBox::is_in(SDL_Rect const& rect1, SDL_Rect const& rect2) const
{
	if (rect1.x > (rect2.x + rect2.w))
	{
		return false;
	}

	if ((rect1.x + rect1.w) < rect2.x)
	{
		return false;
	}

	if (rect1.y > (rect2.y + rect2.h))
	{
		return false;
	}

	if ((rect1.y + rect1.h) < rect2.y)
	{
		return false;
	}

	return true;
}


bool CollisionBox::is_outside_of(SDL_Rect const& rect1, SDL_Rect const& rect2) const
{
	if (rect1.x < rect2.x)
	{
		return true;
	}

	if (rect1.y < rect2.y)
	{
		return true;
	}

	if ((rect1.x + rect1.w) > (rect2.x + rect2.w))
	{
		return true;
	}

	if ((rect1.y + rect1.h) > (rect2.y + rect2.h))
	{
		return true;
	}

	return false;
}
