#include "../headers/Sprite.h"

using namespace std;

Sprite::Sprite(int const& x, int const& y, SDL_Renderer* const& r)
	: head(x, y, "src/images/entity/body.png", r),
	  left_shoulder(x, y, "src/images/entity/body.png", r),
	  right_shoulder(x, y, "src/images/entity/body.png", r)
{
	head.get_hitbox().w = 26;
	head.get_hitbox().h = 26;
	head.get_renderbox().w = 26;
	head.get_renderbox().h = 26;

	left_shoulder.get_hitbox().w = 13;
	left_shoulder.get_hitbox().h = 13;
	left_shoulder.get_renderbox().w = 13;
	left_shoulder.get_renderbox().h = 13;

	right_shoulder.get_hitbox().w = 13;
	right_shoulder.get_hitbox().h = 13;
	right_shoulder.get_renderbox().w = 13;
	right_shoulder.get_renderbox().h = 13;

	left_x = -13;
	left_y = 6;
	l_speed = -0.2;

	right_x = 26;
	right_y = 6;
	r_speed = 0.2;
}

Sprite::~Sprite() {}

void Sprite::input(int new_direction, int const& old_direction)
{
	if (new_direction == Sprite::NORTH || new_direction == Sprite::SOUTH)
	{
		if (old_direction == Sprite::WEST || old_direction == Sprite::EAST)
		{
			swap(left_x, left_y);
			swap(right_x, right_y);
		}
	}
	else if (new_direction == Sprite::EAST || new_direction == Sprite::WEST)
	{
		if (old_direction == Sprite::NORTH || old_direction == Sprite::SOUTH)
		{
			swap(left_x, left_y);
			swap(right_x, right_y);
		}
	}
	else if (new_direction == Sprite::STATIONARY)
	{
		left_x = 6;
		left_y = -13;

		right_x = 6;
		right_y = 26;

		l_speed = -0.2;
		r_speed = 0.2;

		if (old_direction == Sprite::NORTH || old_direction == Sprite::SOUTH)
		{
			swap(left_x, left_y);
			swap(right_x, right_y);
		}
	}
}

void Sprite::update(int const& direction, int const& x, int const& y)
{
	head.get_hitbox().x = x;
	head.get_hitbox().y = y;

	if (direction == Sprite::NORTH || direction == Sprite::SOUTH)
	{
		if (left_shoulder.get_hitbox().y < y || left_shoulder.get_hitbox().y > y + (head.get_hitbox().h / 2))
		{
			l_speed = -l_speed;
		}
		left_y += l_speed;

		r_speed = -l_speed;
		right_y += r_speed;
	}
	else if (direction == Sprite::WEST || direction == Sprite::EAST)
	{
		if (left_shoulder.get_hitbox().x < x || left_shoulder.get_hitbox().x > x + (head.get_hitbox().w / 2))
		{
			l_speed = -l_speed;
		}
		left_x += l_speed;

		r_speed = -l_speed;
		right_x += r_speed;
	}

	left_shoulder.get_hitbox().x = x + left_x;
	left_shoulder.get_hitbox().y = y + left_y;

	right_shoulder.get_hitbox().x = x + right_x;
	right_shoulder.get_hitbox().y = y + right_y;
}

void Sprite::render(SDL_Renderer* const& r) const
{
	left_shoulder.render(r);
	right_shoulder.render(r);
	head.render(r);
}

void Sprite::change_head_color(int const& r, int const& g, int const& b)
{
	head.change_color(r, g, b);
}

void Sprite::change_armor_color(int const& r, int const& g, int const& b)
{
	left_shoulder.change_color(r, g, b);
	right_shoulder.change_color(r, g, b);
}

int Sprite::get_width()
{
	return head.get_hitbox().w;
}

int Sprite::get_height()
{
	return head.get_hitbox().h;
}

SDL_Rect Sprite::get_hitbox(int const& direction)
{
	SDL_Rect temp;
	if (direction == Sprite::NORTH || direction == Sprite::SOUTH)
	{
		temp.x = head.get_hitbox().x - left_shoulder.get_hitbox().w;
		temp.y = head.get_hitbox().y;
		temp.w = head.get_hitbox().w + (right_shoulder.get_hitbox().w * 2);
		temp.h = head.get_hitbox().h;
	}
	else
	{
		temp.x = head.get_hitbox().x;
		temp.y = head.get_hitbox().y - left_shoulder.get_hitbox().h;
		temp.w = head.get_hitbox().w;
		temp.h = head.get_hitbox().h + (right_shoulder.get_hitbox().h * 2);
	}
	return temp;
}
