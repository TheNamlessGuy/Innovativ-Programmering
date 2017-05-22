#ifndef HEADERS_COLLISIONBOX_H_
#define HEADERS_COLLISIONBOX_H_

#include <SDL2/SDL.h>

/**
 * CollisionBox contains all the collision detection used within the game.
 * This is an abstract class that gets inherited by everything that need some sort
 * of collision checking.
 */

class CollisionBox
{
public:
	CollisionBox();
	virtual ~CollisionBox();

	bool is_in(SDL_Rect const& rect1, SDL_Rect const& rect2) const;
	bool is_outside_of(SDL_Rect const& rect1, SDL_Rect const& rect2) const;
};

#endif
