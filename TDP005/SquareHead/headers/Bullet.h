#ifndef HEADERS_BULLET_H_
#define HEADERS_BULLET_H_

#include <SDL2/SDL.h>
#include <vector>

#include "Image.h"
#include "CollisionBox.h"

/**
 * The Bullet class represents a single bullet,
 * with x and y coordinates and the speed it moves in both directions.
 *
 * Friendly bullets are colored black, while enemy bullets are colored red.
 */
class Bullet: public CollisionBox
{
public:
	static const int SPEED;

	Bullet(int const& x_i, int const& y_i, int const& x_speed_i, int const& y_speed_i, bool const& friendly_i);
	virtual ~Bullet();

	/**
	 * Updates the bullet's position based on the speed.
	 * Also checks if the bullet is out of bounds.
	 */
	void update();
	void render(SDL_Renderer* const& r, bool const& debug) const;
	bool is_out_of_bounds() const;

	SDL_Rect get_hitbox() const;

	/**
	 * is_friendly returns true if the bullet was shot by the player.
	 */
	bool is_friendly() const;
protected:
	int x;
	int y;

	const int x_speed;
	const int y_speed;

	bool gone;
	const bool friendly;

	int r;
	int g;
	int b;
};

#endif
