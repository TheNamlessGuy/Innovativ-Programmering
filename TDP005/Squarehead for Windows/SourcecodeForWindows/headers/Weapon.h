#ifndef HEADERS_WEAPON_H_
#define HEADERS_WEAPON_H_

#include <vector>
#include <SDL2/SDL.h>

#include "Bullet.h"

/**
 * The Weapon class represents a weapon held by the player or an enemy.
 */
class Weapon
{
public:
	static const int PISTOL = 0;
	static const int RIFLE = 1;
	static const int SHOTGUN = 2;
	static const int GRENADE = 3;

	Weapon(int type, bool const& friendsly);
	~Weapon();

	void shoot(std::vector<Bullet*>& bullets, int const& x, int const& y, int const& direction);
	/**
	 * get_shot_location returns the location the bullets will spawn at
	 * depending on the direction the entity shooting is facing,
	 * and the current weapon type.
	 */
	std::pair<int, int> get_shot_location(int const& x, int const& y, int const& width, int const& height, int const& direction) const;

	/**
	 * Changes the current weapon type to the parameter.
	 */
	void set_type(int new_type);
private:
	/**
	 * get_bullet_direction returns the x and y speeds of the bullet being shot
	 * depending on which direction the entity shooting is facing.
	 */
	std::pair<int, int> get_bullet_direction(int const& direction) const;
	/**
	 * get_rifle_location returns the specific positions the rifle bullets spawn at.
	 */
	std::vector<std::pair<int, int>> get_rifle_loc(int const& x, int const& y, int const& direction) const;
	/**
	 * get_shotgun_speed returns the x and y speeds of all the bullets fired by the shotgun.
	 */
	std::vector<std::pair<int, int>> get_shotgun_speed(int const& direction) const;

	bool has_grenades(std::vector<Bullet*> const& bullets) const;

	int weapon_type;
	const bool friendly;
};

#endif
