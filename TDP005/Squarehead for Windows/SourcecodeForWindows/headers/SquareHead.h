#ifndef HEADERS_SQUAREHEAD_H_
#define HEADERS_SQUAREHEAD_H_

#include <SDL2/SDL.h>
#include <string>
#include <SDL2/SDL_ttf.h>

/**
 * The SquareHead class represents the main game.
 *
 * It contains the main game loop, as well as handling player input
 * and FPS control.
 */
class SquareHead
{
public:
	static const int CHEAT_DEBUG = 0;
	static const int CHEAT_FRAMEDROP = 1;
	static const int CHEAT_IMMORTAL = 2;

	SquareHead();
	~SquareHead();

	int run();
private:
	/**
	 * Reads the input for the current loop and updates the "KEYS" and "mousedown" variables accordingly.
	 */
	void user_input(SDL_Event e);
	/**
	 * check_cheats checks if the user has inputed any cheats, and activates them accordingly.
	 */
	void check_cheats();

	std::string cheat_string;

	int current_state;
	int new_state;
	int target_frame_delay;

	bool KEYS[322];
	bool cheats[3];
	bool mousedown;
	bool quit_by_x;

	static const std::pair<int, int> SCREEN_SIZE;

	TTF_Font* title_font;
	TTF_Font* font;
};

#endif
