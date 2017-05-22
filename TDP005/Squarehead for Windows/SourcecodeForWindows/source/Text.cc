#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>

#include "../headers/Text.h"

using namespace std;

Text::Text(int const& x_i, int const& y_i, string const& text, SDL_Renderer* const& r, TTF_Font* const& font_i)
	: texture(nullptr), x(x_i), y(y_i), font(font_i)
{
	hitbox.x = x_i;
	hitbox.y = y_i;
	get_texture(text, r);
}

Text::~Text()
{
	font = nullptr;
}

void Text::get_texture(string const& text, SDL_Renderer* const& r)
{
	SDL_DestroyTexture(texture);
	texture = nullptr;

	SDL_Color textcol = {255, 255, 255};
	SDL_Surface* temp = TTF_RenderText_Solid(font, text.c_str(), textcol);
	texture = SDL_CreateTextureFromSurface(r, temp);
	hitbox.w = temp->w;
	hitbox.h = temp->h;
	SDL_FreeSurface(temp);
}

void Text::render(SDL_Renderer* const& r) const
{
	SDL_RenderCopy(r, texture, nullptr, &hitbox);
}

void Text::update_string(string text, SDL_Renderer* r)
{
	get_texture(text, r);
}

void Text::update_char(char c, SDL_Renderer* r)
{
	ostringstream oss;
	oss << c;

	update_string(oss.str(), r);
}
