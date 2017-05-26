#ifndef HEADERS_DOOR_H_
#define HEADERS_DOOR_H_

#include <SDL2/SDL.h>

#include "Image.h"
#include "Enemy.h"

/**
 * The Door class represents a door that can open,
 * shut and spawn enemies at a fixed point behind the door.
 *
 * The door can either be vertical or horizontal.
 */
class Door
{
public:

	static const int movespeed = 1;

	Door(int const& x_i, int const& y_i, bool const& vertical, SDL_Renderer* r);
	~Door();

	void spawn_enemy(std::vector<Enemy*>&  v, SDL_Renderer* const& r);
	void update();
	void render(SDL_Renderer* const& r) const;

	void open();
	void close();

	/**
	 * Reset resets the doors to the original position set within the constructor.
	 *
	 * Is used so that if you start a new game, the doors won't be open.
	 */
	void reset();
private:
	/**
	 * The DoorPart class is a square with an Image, which represents the two
	 * parts of the door that open and close.
	 */
	class DoorPart
	{
	public:
		DoorPart(SDL_Renderer* const& r, bool const& is_vertical);
		~DoorPart();

		Image* img;
	};

	int x;
	int y;
	int a_orig;
	int spawn_point;
	bool is_vertical;

	DoorPart a;
	DoorPart b;

	bool is_opening;
	bool is_closing;
	int amount_spawning;
};

#endif
