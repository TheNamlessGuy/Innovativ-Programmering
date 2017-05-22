#include <vector>
#include <SDL2/SDL.h>

#include "../headers/Weapon.h"
#include "../headers/Bullet.h"
#include "../headers/Player.h"
#include "../headers/Grenade.h"

using namespace std;

Weapon::Weapon(int type, bool const& friendly_i)
	: weapon_type(type), friendly(friendly_i)
{}

Weapon::~Weapon() {}

void Weapon::shoot(vector<Bullet*>& bullets, int const& x, int const& y, int const& direction)
{
	switch(weapon_type)
	{
	case Weapon::PISTOL:
		{
			pair<int, int> bullet_direction = get_bullet_direction(direction);

			bullets.push_back(new Bullet(x, y, bullet_direction.first, bullet_direction.second, friendly));
			break;
		}
	case Weapon::RIFLE:
		{
			pair<int, int> bullet_direction = get_bullet_direction(direction);
			vector<pair<int, int>> location = get_rifle_loc(x, y, direction);

			for (int i = 0; i < 3; ++i)
			{
				bullets.push_back(new Bullet(location.at(i).first, location.at(i).second,
						bullet_direction.first, bullet_direction.second, friendly));
			}
			break;
		}
	case Weapon::SHOTGUN:
		{
			vector<pair<int, int>> shotgun_direction = get_shotgun_speed(direction);

			for (int i = 0; i < 3; ++i)
			{
				bullets.push_back(new Bullet(x, y, shotgun_direction.at(i).first,
						shotgun_direction.at(i).second, friendly));
			}
			break;
		}
	case Weapon::GRENADE:
		{
			if (has_grenades(bullets))
			{
				break;
			}

			pair<int, int> bullet_direction = get_bullet_direction(direction);

			bullets.push_back(new Grenade(x, y, bullet_direction.first, bullet_direction.second, friendly));
			break;
		}
	default:
		break;
	}
}

pair<int, int> Weapon::get_bullet_direction(int const& direction) const
{
	pair<int, int> return_pair;

	switch (direction)
	{
	case Sprite::NORTH:
		return_pair.first = 0;
		return_pair.second = -Bullet::SPEED;
		break;
	case Sprite::SOUTH:
		return_pair.first = 0;
		return_pair.second = Bullet::SPEED;
		break;
	case Sprite::WEST:
		return_pair.first = -Bullet::SPEED;
		return_pair.second = 0;
		break;
	case Sprite::EAST:
		return_pair.first = Bullet::SPEED;
		return_pair.second = 0;
		break;
	case Sprite::NORTHWEST:
		return_pair.first = -Bullet::SPEED / 2;
		return_pair.second = -Bullet::SPEED / 2;
		break;
	case Sprite::NORTHEAST:
		return_pair.first = Bullet::SPEED / 2;
		return_pair.second = -Bullet::SPEED / 2;
		break;
	case Sprite::SOUTHWEST:
		return_pair.first = -Bullet::SPEED / 2;
		return_pair.second = Bullet::SPEED / 2;
		break;
	case Sprite::SOUTHEAST:
		return_pair.first = Bullet::SPEED / 2;
		return_pair.second = Bullet::SPEED / 2;
		break;
	default:
		break;
	}

	return return_pair;
}

vector<pair<int, int>> Weapon::get_rifle_loc(int const& x, int const& y, int const& direction) const
{
	vector<pair<int, int>> return_vector;

	return_vector.push_back(make_pair(x, y));

	if (direction == Sprite::NORTH || direction == Sprite::SOUTH)
	{
		return_vector.push_back(make_pair(x, y + 10));
		return_vector.push_back(make_pair(x, y - 10));
	}
	else
	{
		return_vector.push_back(make_pair(x + 10, y));
		return_vector.push_back(make_pair(x - 10, y));
	}

	return return_vector;
}

vector<pair<int, int>> Weapon::get_shotgun_speed(int const& direction) const
{
	vector<pair<int, int>> return_vector;

	switch(direction)
	{
	case Sprite::NORTH:
		//Northwest
		return_vector.push_back(make_pair(-Bullet::SPEED, -Bullet::SPEED));

		//North
		return_vector.push_back(make_pair(0, -Bullet::SPEED));

		//Northeast
		return_vector.push_back(make_pair(Bullet::SPEED, -Bullet::SPEED));
		break;
	case Sprite::SOUTH:
		//Southwest
		return_vector.push_back(make_pair(-Bullet::SPEED, Bullet::SPEED));

		//South
		return_vector.push_back(make_pair(0, Bullet::SPEED));

		//Southeast
		return_vector.push_back(make_pair(Bullet::SPEED, Bullet::SPEED));
		break;
	case Sprite::WEST:
		//Northwest
		return_vector.push_back(make_pair(-Bullet::SPEED, -Bullet::SPEED));

		//West
		return_vector.push_back(make_pair(-Bullet::SPEED, 0));

		//Southwest
		return_vector.push_back(make_pair(-Bullet::SPEED, Bullet::SPEED));
		break;
	case Sprite::EAST:
		//Northeast
		return_vector.push_back(make_pair(Bullet::SPEED, -Bullet::SPEED));

		//East
		return_vector.push_back(make_pair(Bullet::SPEED, 0));

		//Southeast
		return_vector.push_back(make_pair(Bullet::SPEED, Bullet::SPEED));
		break;
	default:
		break;
	}

	return return_vector;
}

pair<int, int> Weapon::get_shot_location(int const& x, int const& y,
		int const& width, int const& height, int const& direction) const
{
	pair<int, int> location;

	switch(direction)
	{
		case Sprite::NORTH:
			location.first = (x + (width / 2));
			location.second = y;
			break;
		case Sprite::SOUTH:
			location.first = (x + (width / 2));
			location.second = (y + height);
			break;
		case Sprite::WEST:
			location.first = x;
			location.second = (y + (height / 2));
			break;
		case Sprite::EAST:
			location.first = (x + width);
			location.second = (y + (height / 2));
			break;
		default:
			break;
		}

	return location;
}

void Weapon::set_type(int new_type)
{
	weapon_type = new_type;
}

bool Weapon::has_grenades(vector<Bullet*> const& bullets) const
{
	for (Bullet* bullet : bullets)
	{
		Grenade* grenade = dynamic_cast<Grenade*>(bullet);

		if (grenade != nullptr)
		{
			return true;
		}
	}
	return false;
}
