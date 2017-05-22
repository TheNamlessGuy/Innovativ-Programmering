#ifndef HEADERS_SPRITE_H_
#define HEADERS_SPRITE_H_

#include <SDL2/SDL.h>

#include "Image.h"

/**
 * Handles the animation and images of the objects of the classes that extend Entity.
 */
class Sprite
{
public:
	static const int STATIONARY = -1;
	static const int NORTH = 0;
	static const int SOUTH = 1;
	static const int WEST = 2;
	static const int EAST = 3;
	static const int NORTHWEST = 4;
	static const int NORTHEAST = 5;
	static const int SOUTHWEST = 6;
	static const int SOUTHEAST = 7;

	Sprite(int const& x_i, int const& y, SDL_Renderer* const& r);
	~Sprite();

	/**
	 * input turns the entity and updates the animation if the entity is moving.
	 */
	void input(int new_direction, int const& old_direction);
	void update(int const& direction, int const& x, int const& y);
	void render(SDL_Renderer* const& r) const;

	void change_head_color(int const& r, int const& g, int const& b);
	/**
	 * change_armor_color changes the color of the left_shoulder and the right_shoulder
	 * to the inputed RGB value.
	 */
	void change_armor_color(int const& r, int const& g, int const& b);

	int get_width();
	int get_height();

	SDL_Rect get_hitbox(int const& direction);
private:
	Image head;

	Image left_shoulder;
	Image right_shoulder;

	double left_x;
	double left_y;
	double l_speed;

	double right_x;
	double right_y;
	double r_speed;
};

#endif
