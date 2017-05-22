#include "../headers/Bullet.h"
#include "../headers/Image.h"
#include "../headers/CollisionBox.h"

using namespace std;

const int Bullet::SPEED = 5;

Bullet::Bullet(int const& x_i, int const& y_i,
		int const& x_speed_i, int const& y_speed_i, bool const& friendly_i)
	: CollisionBox(), x(x_i), y(y_i), x_speed(x_speed_i),
	  y_speed(y_speed_i), gone(false), friendly(friendly_i)
{
	if (friendly)
	{
		r = 0;
		g = 0;
		b = 0;
	}
	else
	{
		r = 255;
		g = 0;
		b = 0;
	}
}

Bullet::~Bullet() {}

void Bullet::update()
{
	x += x_speed;
	if (x > 800 || x < 0)
	{
		gone = true;
	}

	y += y_speed;
	if (y > 600 || y < 0)
	{
		gone = true;
	}
}

void Bullet::render(SDL_Renderer* const& ren, bool const& debug) const
{
	SDL_SetRenderDrawColor(ren, r, g, b, 255);
	SDL_RenderDrawLine(ren, x - 2, y, x + 2, y);
	SDL_RenderDrawLine(ren, x, y - 2, x, y + 2);
	SDL_RenderDrawLine(ren, x - 1, y - 1, x + 1, y + 1);
	SDL_RenderDrawLine(ren, x - 1, y + 1, x + 1, y - 1);
	if (debug)
	{
		SDL_Rect temp = get_hitbox();
		SDL_SetRenderDrawColor(ren, 0, 255, 255, 255);
		SDL_RenderDrawRect(ren, &temp);
	}
}

bool Bullet::is_out_of_bounds() const
{
	return gone;
}

SDL_Rect Bullet::get_hitbox() const
{
	SDL_Rect temp;
	temp.x = x - 2;
	temp.y = y - 2;
	temp.h = 5;
	temp.w = 5;
	return temp;
}

bool Bullet::is_friendly() const
{
	return friendly;
}
