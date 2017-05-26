#ifndef HEADERS_MAINMENUSTATE_H_
#define HEADERS_MAINMENUSTATE_H_

#include <SDL2/SDL.h>

#include "GameState.h"
#include "Button.h"
#include "Text.h"

/**
 * The MainMenuState is the state the game starts in.
 *
 * It contains two buttons: one for starting the game, and one for quitting.
 */
class MainMenuState: public GameState
{
public:
	MainMenuState(SDL_Renderer* const& r, std::pair<int, int> const& screen_size, TTF_Font* const& font);
	~MainMenuState();

	void reset(SDL_Renderer* const& r);

	void input(bool KEYS[], bool const& mousedown, SDL_Renderer* const& r);
	int update(long long& score, SDL_Renderer* const& r, bool cheats[]);
	void render(SDL_Renderer* const& r, bool const& debug);

private:
	Button play;
	Button quit;

	Text* gamename;
};

#endif
