#ifndef HEADERS_TEXT_H_
#define HEADERS_TEXT_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

/**
 * The Text class represents a string that is able to render in SDL2.
 */
class Text
{
public:
	Text(int const& x_i, int const& y_i, std::string const& text, SDL_Renderer* const& r, TTF_Font* const& font_i);
	~Text();

	/**
	 * Updates the text in the Text object to the string inputed.
	 */
	void update_string(std::string text, SDL_Renderer* r);
	/**
	 * Updates the text in the Text object to the char inputed.
	 */
	void update_char(char c, SDL_Renderer* r);
	void render(SDL_Renderer* const& r) const;
private:
	/**
	 * Creates a texture based on the string parameter.
	 */
	void get_texture(std::string const& text, SDL_Renderer* const& r);

	SDL_Texture* texture;
	SDL_Rect hitbox;

	int x;
	int y;

	TTF_Font* font;
};

#endif
