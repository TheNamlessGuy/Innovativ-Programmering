#ifndef HEADERS_PLAYER_H_
#define HEADERS_PLAYER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Bullet.h"
#include "Image.h"
#include "Weapon.h"
#include "Sprite.h"
#include "Entity.h"
#include "CollisionBox.h"

/**
 * The Player class represents the playable character controlled by the keyboard.
 */
class Player: public CollisionBox, public Entity
{
public:
	Player(int const& x, int const& y, SDL_Renderer* const& r);
	~Player();

	void input(bool KEYS[], std::vector<Bullet*>& bullets, SDL_Rect playscreen);
	void update(long long score, int armor, bool rapidfire, bool slowfire);
	void render(SDL_Renderer* const& r, bool const& debug);

	/**
	 * Returns a square around the player which the enemies try to stay out of.
	 */
	SDL_Rect get_aura();

	void set_weapon(int new_type);

	/**
	 * Returns true if one of the bullets in the parameter 'bullets' is colliding with the player.
	 *
	 * If one of the bullets are colliding, it is removed from the list.
	 */
	bool is_hit(std::vector<Bullet*>& bullets);

	void reset();
private:
	int movespeed;

	double shoot_timer;

	/**
	 * change_armor_color changes the color of the player's armor depending on
	 * how many armor-powerups the player currently has.
	 */
	void change_armor_color(int new_armor);
	/**
	 * change_head_color changes the color of the player's
	 * head depending on the player's current score.
	 */
	void change_head_color(long long const& score);
};

#endif
