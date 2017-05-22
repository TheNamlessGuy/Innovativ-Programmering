#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#include "../headers/Image.h"

using namespace std;

Image::Image(int const& x_i, int const& y_i, string const& filename, SDL_Renderer* const& r)
{
	hitbox.x = x_i;
	hitbox.y = y_i;
	texture = nullptr;

	{
		SDL_Surface* temp = IMG_Load(filename.c_str());
		texture = SDL_CreateTextureFromSurface(r, temp);
		hitbox.h = temp->h;
		hitbox.w = temp->w;
		SDL_FreeSurface(temp);
	}

	renderbox.x = 0;
	renderbox.y = 0;
	renderbox.h = hitbox.h;
	renderbox.w = hitbox.w;
}

Image::~Image()
{
	SDL_DestroyTexture(texture);
}

void Image::render(SDL_Renderer* const& r) const
{
	SDL_RenderCopy(r, texture, &renderbox, &hitbox);
}

void Image::render(SDL_Renderer* const& r, int const& x_i, int const& y_i)
{
	hitbox.x = x_i;
	hitbox.y = y_i;

	render(r);
}

void Image::render(SDL_Renderer* const& r, SDL_Rect const& box) const
{
	SDL_RenderCopy(r, texture, &renderbox, &box);
}

SDL_Rect& Image::get_hitbox()
{
	return hitbox;
}

SDL_Rect& Image::get_renderbox()
{
	return renderbox;
}

void Image::change_color(int const& r_i, int const& g_i, int const& b_i) const
{
	SDL_SetTextureColorMod(texture, r_i, g_i, b_i);
}
