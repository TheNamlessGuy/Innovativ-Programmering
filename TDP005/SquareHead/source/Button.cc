#include <vector>
#include <string>
#include <SDL2/SDL.h>

#include "../headers/Button.h"

using namespace std;

Button::Button(int const& x_i, int const& y_i, string const& filename, SDL_Renderer* const& r)
	: not_hover_img(new Image(x_i, y_i, filename, r)),
	  hover_img(new Image(x_i, y_i, filename, r)),
	  x(x_i),
	  y(y_i)
{
	current = not_hover_img;

	not_hover_img->get_renderbox().h /= 2;
	not_hover_img->get_hitbox().h /= 2;

	hover_img->get_renderbox().y = hover_img->get_hitbox().h / 2;
	hover_img->get_hitbox().h /= 2;

	width = current->get_hitbox().w;
	height = current->get_hitbox().h;
}

Button::~Button()
{
	delete not_hover_img;
	delete hover_img;
}

void Button::render(SDL_Renderer* const& r) const
{
	current->render(r);
}

void Button::set_hover(bool const& hovering)
{
	if (hovering)
	{
		current = hover_img;
	}
	else
	{
		current = not_hover_img;
	}
}

bool Button::is_hover(int const& mouse_x, int const& mouse_y) const
{
	return ((mouse_x >= x && mouse_x <= (x + width)) &&
			(mouse_y >= y && mouse_y <= (y + height)));
}
