#include <SDL2/SDL.h>
#include <vector>
#include <time.h>
#include <algorithm>
#include <cmath>
#include <iostream>

#include "../headers/Enemy.h"
#include "../headers/Bullet.h"
#include "../headers/Entity.h"

using namespace std;

double Enemy::enemy_speed = 1;

Enemy::Enemy(int const& x_i, int const& y_i, SDL_Renderer* const& r): CollisionBox(),
		Entity(x_i, y_i, Sprite::NORTH, r, false),
		shoot_timer(0), stab_timer(0), turn_timer(0)
{
	sprite.change_armor_color(0, 0, 255);
	sprite.change_head_color(0, 0, 0);
}

Enemy::~Enemy() {}

void Enemy::update(vector<Bullet*>& bullets, SDL_Rect const& playscreen,
		SDL_Rect const& player, SDL_Rect const& player_aura, bool& stabbed)
{
	shoot_timer -= 0.1;
	stab_timer -= 0.1;
	turn_timer -= 0.1;

	vector<pair<int, int>> points;
	pair<int, int> closest_point;

	points.push_back(make_pair(player_aura.x + (player_aura.w / 2),
				player_aura.y));
	points.push_back(make_pair(player_aura.x + player_aura.w,
				player_aura.y + (player_aura.h / 2)));
	points.push_back(make_pair(player_aura.x + (player_aura.w / 2),
				player_aura.y + player_aura.h));
	points.push_back(make_pair(player_aura.x,
				player_aura.y + (player_aura.h / 2)));

	closest_point = points.back();

	for (int i = 0; i < 4; ++i)
	{
		if (get_distance_to(points.back()) <= get_distance_to(closest_point)){
			closest_point = points.back();
		}
		points.pop_back();
	}

	if (is_in(player, get_hitbox()) && stab_timer <= 0)
	{
		stabbed = true;
		stab_timer = 3;
	}

	SDL_Rect temp;
	temp.x = closest_point.first-1;
	temp.y = closest_point.second-1;
	temp.w = 3;
	temp.h = 3;

	if (!is_in(temp, get_hitbox()))
	{
		move_towards(temp);
	}

	int shot_direction = get_shot_direction(player);
	if (shot_direction != -1 && shoot_timer <= 0)
	{
		shoot(bullets, shot_direction);
		shoot_timer = 5;
	}

	sprite.update(facing, x, y);
}

void Enemy::render(SDL_Renderer* const& r, bool const& debug)
{
	if (debug)
	{
		SDL_Rect temp = get_hitbox();
		SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
		SDL_RenderDrawRect(r, &temp);
	}

	sprite.render(r);
}

int Enemy::get_shot_direction(SDL_Rect const& to_shoot)
{
	pair<int, int> bullet_spawn_loc = weapon.get_shot_location(x, y, sprite.get_width(), sprite.get_height(), facing);

	if (facing == Sprite::NORTH &&
			(bullet_spawn_loc.first > to_shoot.x &&
			bullet_spawn_loc.first < to_shoot.x + to_shoot.w))
	{
		return Sprite::NORTH;
	}
	else if (facing == Sprite::WEST &&
			(bullet_spawn_loc.second > to_shoot.y &&
			bullet_spawn_loc.second < to_shoot.y + to_shoot.h))
	{
		return Sprite::WEST;
	}
	else if (facing == Sprite::SOUTH &&
			(bullet_spawn_loc.first > to_shoot.x &&
			bullet_spawn_loc.first < to_shoot.x + to_shoot.w))
	{
		return Sprite::SOUTH;
	}
	else if (facing == Sprite::EAST &&
			(bullet_spawn_loc.second > to_shoot.y &&
			bullet_spawn_loc.second < to_shoot.y + to_shoot.h))
	{
		return Sprite::EAST;
	}
	else
	{
		pair<int, int> distance;
		distance.first = bullet_spawn_loc.first - (to_shoot.x + (to_shoot.w / 2));
		distance.second = bullet_spawn_loc.second - (to_shoot.y + (to_shoot.h / 2));

		int dir;

		if (distance.first > 0)
		{
			if (distance.second > 0)
			{
				dir = Sprite::NORTHWEST;
			}
			else
			{
				dir = Sprite::SOUTHWEST;
			}
		}
		else
		{
			if (distance.second > 0)
			{
				dir = Sprite::NORTHEAST;
			}
			else
			{
				dir = Sprite::SOUTHEAST;
			}
		}

		distance.first = abs(distance.first);
		distance.second = abs(distance.second);

		if (abs(distance.first - distance.second) < 45)
		{
			return dir;
		}
		return -1;
	}
}

void Enemy::move_towards(SDL_Rect const& target)
{
	if (abs(int(target.x - x)) > abs(int(target.y - y)))
	{
		if ((target.x - x) > 0)
		{
			x += enemy_speed;
			if (turn_timer <= 0)
			{
				sprite.input(Sprite::EAST, facing);
				facing = Sprite::EAST;
				turn_timer = 5;
			}
		}
		else
		{
			x -= enemy_speed;
			if (turn_timer <= 0)
			{
				sprite.input(Sprite::WEST, facing);
				facing = Sprite::WEST;
				turn_timer = 5;
			}
		}
	}
	else if (abs(int(target.x - x)) < abs(int(target.y - y)))
	{
		if ((target.y - y) > 0)
		{
			y += enemy_speed;
			if (turn_timer <= 0)
			{
				sprite.input(Sprite::SOUTH, facing);
				facing = Sprite::SOUTH;
				turn_timer = 5;
			}
		}
		else
		{
			y -= enemy_speed;
			if (turn_timer <= 0)
			{
				sprite.input(Sprite::NORTH, facing);
				facing = Sprite::NORTH;
				turn_timer = 5;
			}
		}
	}
	else
	{
		sprite.input(Sprite::STATIONARY, facing);
	}
}

bool Enemy::is_hit(vector<Bullet*>& bullets, bool const& penetrating)
{
	vector<Bullet*>::iterator iter = bullets.begin();
	for (; iter != bullets.end(); ++iter)
	{
		if ((*iter)->is_friendly() && is_in(get_hitbox(), (*iter)->get_hitbox()))
		{
			if (!penetrating)
			{
				delete (*iter);
				bullets.erase(iter);
			}
			return true;
		}
	}
	return false;
}

int Enemy::get_distance_to(pair<int, int> const& a)
{
	int x_diff = get_hitbox().x - a.first;
	int y_diff = get_hitbox().y - a.second;
	return static_cast<int>(sqrt((x_diff * x_diff) + (y_diff * y_diff)));
}

bool Enemy::drop_pickup() const
{
	//random_device rd;
	//uniform_int_distribution<int> uid(0, 4);

	srand(time(NULL));
	return !(rand()%5);
}
