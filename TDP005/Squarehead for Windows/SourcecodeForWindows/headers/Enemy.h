#ifndef HEADERS_ENEMY_H_
#define HEADERS_ENEMY_H_

#include <SDL2/SDL.h>
#include <vector>

#include "Sprite.h"
#include "Bullet.h"
#include "Weapon.h"
#include "CollisionBox.h"
#include "Entity.h"

/**
 * The Enemy class represents a single enemy and its AI.
 *
 * The AI can handle walking, shooting and randomly dropping pickups upon death.
 */

class Enemy: public CollisionBox, public Entity
{
public:

	static double enemy_speed;

	Enemy(int const& x_i, int const& y_i, SDL_Renderer* const& r);
	~Enemy();

	/**
	 * The update function contains the majority of the enemy's AI, such as
	 * shooting, walking and stabbing.
	 */
	void update(std::vector<Bullet*>& bullets, SDL_Rect const& playscreen,
			SDL_Rect const& player_hitbox, SDL_Rect const& player_aura, bool& stabbed);
	void render(SDL_Renderer* const& r, bool const& debug);

	/**
	 * 'is_hit' returns true if one of the bullets within the 'bullets' parameter
	 * is colliding with the enemy.
	 *
	 * If the bullet is colliding, the function also removes it from the vector,
	 * unless 'penetrating' is true.
	 */
	bool is_hit(std::vector<Bullet*>& bullets, bool const& penetrating);
	/**
	 * Returns true on a 1/5 ratio (the droprate for a pickup).
	 */
	bool drop_pickup() const;
private:
	double shoot_timer;
	double stab_timer;
	double turn_timer;

	/**
	 * Get shot direction returns a static int constant representing a direction
	 * which the enemy has to shoot in order to hit the player (the parameter 'to_hit').
	 */
	int get_shot_direction(SDL_Rect const& to_hit);
	int get_distance_to(std::pair<int, int> const& a);

	void move_towards(SDL_Rect const& target);
};

#endif
