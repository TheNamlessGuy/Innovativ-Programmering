#ifndef HEADERS_ENTITY_H_
#define HEADERS_ENTITY_H_

#include <vector>
#include <SDL2/SDL.h>

#include "Bullet.h"
#include "Sprite.h"
#include "Weapon.h"

/**
 * An abstract class representing all "living" objects on the screen.
 *
 * Contains functions and variables used by both the Player class and the Enemy class.
 */
class Entity
{
public:
	Entity(int const& x, int const& y, int facing, SDL_Renderer* const& r, bool const& friendly_i);
	virtual ~Entity();

	virtual void render(SDL_Renderer* const& r, bool const& debug) = 0;
	virtual void shoot(std::vector<Bullet*>& v, int const& shot_direction);

	SDL_Rect get_hitbox();
protected:
	double x;
	double y;
	int facing;

	Sprite sprite;

	Weapon weapon;
};

#endif
