#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../headers/Player.h"
#include "../headers/Weapon.h"

using namespace std;

Player::Player(int const& x_i, int const& y_i, SDL_Renderer* const& r)
	: CollisionBox(),
	  Entity(x_i, y_i, Sprite::NORTH, r, true)
{
	movespeed = 3;

	sprite.change_head_color(160, 82, 45);
	sprite.change_armor_color(0, 100, 0);

	shoot_timer = 0;
}

Player::~Player() {}

void Player::input(bool KEYS[], vector<Bullet*>& bullets, SDL_Rect playscreen)
{
	if (!is_outside_of(get_hitbox(), playscreen))
	{
		if (KEYS[SDLK_w])
		{
			y -= movespeed;
			sprite.input(Sprite::NORTH, facing);
			facing = Sprite::NORTH;
		}
		else if (KEYS[SDLK_s])
		{
			y += movespeed;
			sprite.input(Sprite::SOUTH, facing);
			facing = Sprite::SOUTH;
		}
		else if (KEYS[SDLK_a])
		{
			x -= movespeed;
			sprite.input(Sprite::WEST, facing);
			facing = Sprite::WEST;
		}
		else if (KEYS[SDLK_d])
		{
			x += movespeed;
			sprite.input(Sprite::EAST, facing);
			facing = Sprite::EAST;
		}
		else
		{
			sprite.input(Sprite::STATIONARY, facing);
		}

		if (KEYS[SDLK_SPACE] && shoot_timer <= 0)
		{
			shoot(bullets, facing);
			shoot_timer = 2;
		}
	}
	else
	{
		if (x > playscreen.w / 2)
		{
			x -= movespeed;
		}
		else
		{
			x += movespeed;
		}

		if (y > playscreen.h / 2)
		{
			y -= movespeed;
		}
		else
		{
			y += movespeed;
		}
	}
}

void Player::update(long long score, int armor, bool rapidfire, bool slowfire)
{
	shoot_timer -= 0.1 + (0.07 * rapidfire) - (0.07 * slowfire);

	change_armor_color(armor);
	change_head_color(score);

	sprite.update(facing, x, y);
}

void Player::render(SDL_Renderer* const& r, bool const& debug)
{
	if (debug)
	{
		SDL_Rect temp = get_aura();
		SDL_SetRenderDrawColor(r, 255, 255, 0, 255);
		SDL_RenderDrawRect(r, &temp);

		temp = get_hitbox();
		SDL_SetRenderDrawColor(r, 0, 0, 255, 255);
		SDL_RenderDrawRect(r, &temp);
	}
	sprite.render(r);
}

SDL_Rect Player::get_aura()
{
	SDL_Rect hitbox = get_hitbox();

	hitbox.x -= 50;
	hitbox.y -= 50;
	hitbox.w += 100;
	hitbox.h += 100;

	return hitbox;
}

void Player::set_weapon(int new_type)
{
	weapon.set_type(new_type);
}

bool Player::is_hit(vector<Bullet*>& bullets)
{
	vector<Bullet*>::iterator iter = bullets.begin();

	for (; iter != bullets.end(); ++iter)
	{
		if (!(*iter)->is_friendly() && is_in(get_hitbox(), (*iter)->get_hitbox()))
		{
			delete *iter;
			bullets.erase(iter, iter+1);

			return true;
		}
	}
	return false;
}

void Player::change_armor_color(int armor)
{
	switch (armor)
	{
	case 0:
		sprite.change_armor_color(0, 100, 0);
		break;
	case 1:
		sprite.change_armor_color(255, 255, 0);
		break;
	case 2:
		sprite.change_armor_color(255, 165, 0);
		break;
	case 3:
		sprite.change_armor_color(255, 20, 147);
		break;
	case 4:
		sprite.change_armor_color(138, 43, 226);
		break;
	case 5:
		sprite.change_armor_color(0, 0, 0);
		break;
	default:
		break;
	}
}

void Player::reset()
{
	x = 387;
	y = 287;

	weapon.set_type(Weapon::PISTOL);

	facing = Sprite::NORTH;
}

void Player::change_head_color(long long const& score)
{
	if (score < -2000000)
	{
		sprite.change_head_color(0, 0, 0);
	}
	else if (score < -15000)
	{
		sprite.change_head_color(255, 0, 0);
	}
	else if (score < -10000)
	{
		sprite.change_head_color(220, 69, 0);
	}
	else if (score < -6000)
	{
		sprite.change_head_color(200, 20, 0);
	}
	else if (score < -3000)
	{
		sprite.change_head_color(178, 34, 34);
	}
	else if (score < -1000)
	{
		sprite.change_head_color(128, 0, 0);
	}
	else
	{
		sprite.change_head_color(160, 82, 45);
	}
}
