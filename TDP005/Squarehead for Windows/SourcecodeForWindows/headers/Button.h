#ifndef HEADERS_BUTTON_H_
#define HEADERS_BUTTON_H_

#include <vector>
#include <string>
#include <SDL2/SDL.h>

#include "Image.h"

/**
 * The Button class represents a clickable button,
 * which changes the image depending on if the mouse is hovering over it or not.
 */

class Button
{
public:
	Button(int const& x_i, int const& y_i, std::string const& filename, SDL_Renderer* const& r);
	~Button();

	/**
	 * Returns a bool depending on if the mouse is hovering over the button.
	 */
	bool is_hover(int const& mouse_x, int const& mouse_y) const;

	void set_hover(bool const& hovering);
	void render(SDL_Renderer* const& r) const;
private:
	Image* not_hover_img;
	Image* hover_img;
	Image* current;

	int x;
	int y;
	int width;
	int height;
};

#endif
