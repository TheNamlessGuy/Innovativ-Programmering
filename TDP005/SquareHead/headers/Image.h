#ifndef HEADERS_IMAGE_H_
#define HEADERS_IMAGE_H_

#include <SDL2/SDL.h>
#include <string>

/**
 * The Image class represents a texture and its hitbox.
 * Also contains the part of the image loaded it should render.
 */
class Image
{
public:
	Image(int const& x_i, int const& y_i, std::string const& filename, SDL_Renderer* const& r);
	~Image();

	void render(SDL_Renderer* const& r) const;
	void render(SDL_Renderer* const& r, int const& x_i, int const& y_i);
	void render(SDL_Renderer* const& r, SDL_Rect const& box) const;

	SDL_Rect& get_hitbox();
	SDL_Rect& get_renderbox();

	void change_color(int const& r_i, int const& g_i, int const& b_i) const;
private:
	SDL_Texture* texture;
	SDL_Rect hitbox;
	SDL_Rect renderbox;
};

#endif
