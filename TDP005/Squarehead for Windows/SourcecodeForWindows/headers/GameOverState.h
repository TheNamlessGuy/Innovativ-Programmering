#ifndef HEADERS_GAMEOVERSTATE_H_
#define HEADERS_GAMEOVERSTATE_H_

#include <map>
#include <SDL2/SDL.h>
#include <vector>
#include <SDL2/SDL_ttf.h>

#include "GameState.h"
#include "Text.h"
#include "Button.h"

/**
 * The GameOverState is the state that is displayed when the player dies.
 *
 * It allows the user to update the highscore-list, and return to the main menu.
 */

class GameOverState: public GameState
{
public:
	GameOverState(SDL_Renderer* const& r, TTF_Font* const& font_i);
	~GameOverState();

	void input(bool KEYS[], bool const& mousedown, SDL_Renderer* const& r);
	int update(long long& score, SDL_Renderer* const& r, bool cheats[]);
	void render(SDL_Renderer* const& r, bool const& debug);

	/**
	 * The reset function resets this state so that it can be used again
	 * within the same session.
	 */
	void reset(SDL_Renderer* const& r);

private:
	int curr_char;
	long long private_score;

	Button back_button;

	double input_timer;

	std::vector<Text*> name;
	std::string name_str;

	std::vector<Text*> highscore;

	TTF_Font* font;

	std::vector<std::pair<long long, std::string>> highscore_values;

	std::vector<std::pair<long long, std::string>> read_highscore_from_file();
	/**
	 * create_highscore_text converts the variable "highscore_values" to instances of Text.
	 */
	void create_highscore_text(SDL_Renderer* r);
	/**
	 * update_highscore_file writes the first five values in highscore_values to the highscore file.
	 */
	void update_highscore_file();

	/**
	 * sort_highscore_values sorts the variable "highscore_values" in ascending order.
	 */
	void sort_highscore_values();
	/**
	 * create_highscore_file creates the highscore txt-file if it does not exist.
	 */
	void create_highscore_file();
};

#endif
