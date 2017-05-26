#ifndef HEADERS_GRENADE_H_
#define HEADERS_GRENADE_H_

#include "../headers/Bullet.h"

/**
 * The Grenade class represents the "bullet" that is shot by the "Grenade" weapon.
 *
 * The grenade-bullet creates eight bullets shooting out in different directions
 * a set time after it was created.
 */

class Grenade: public Bullet
{
public:
	Grenade(int const& x_i, int const& y_i, int const& x_speed_i, int const& y_speed_i, bool const& friendly_i);
	~Grenade();

	std::vector<Bullet*> update();
private:
	double fuse_timer;
};

#endif
