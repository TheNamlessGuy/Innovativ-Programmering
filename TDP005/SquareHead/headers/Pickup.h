#ifndef HEADERS_PICKUP_H_
#define HEADERS_PICKUP_H_

#include <SDL2/SDL.h>

#include "Image.h"
#include "CollisionBox.h"

/**
 * The Pickup class represents the little presents an Enemy can drop randomly.
 *
 * When it collides with the player, the pick_up function is called.
 */
class Pickup: public CollisionBox
{
public:
	Pickup(int const& x_i, int const& y_i, SDL_Renderer* const& r);
	~Pickup();

	void render(SDL_Renderer* const& r) const;
	bool update();

	bool is_colliding_with(SDL_Rect const& player);
	/**
	 * Returns a random weapon or powerup when called.
	 *
	 * The player has a 60% chance to get armor,
	 * 20% chance for some kind of weapon,
	 * and 20% chance to get a powerup.
	 *
	 * The first bool in the return pair is true if the returning int is a weapon, otherwise false.
	 */
	std::pair<bool, int> pick_up(long long const& score) const;
private:
	SDL_Rect get_hitbox();
	Image img;

	int x;
	int y;
	int lifespan;
};

#endif
