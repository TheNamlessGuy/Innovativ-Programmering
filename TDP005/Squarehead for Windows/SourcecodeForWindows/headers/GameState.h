#ifndef HEADERS_GAMESTATE_H_
#define HEADERS_GAMESTATE_H_

#include <SDL2/SDL.h>

/**
 * GameState is an abstract class that is inherited by
 * the MainMenuState, the PlayState and the GameOverState.
 *
 * It is used within the main game loop to represent the current state.
 */

class GameState
{
public:
	static const int QUIT = -1;
	static const int MAINMENU = 0;
	static const int PLAY = 1;
	static const int GAMEOVER = 2;

	GameState(int state_i);
	virtual ~GameState();

	virtual void input(bool KEYS[], bool const& mousedown, SDL_Renderer* const& r) = 0;
	virtual int update(long long& score, SDL_Renderer* const& r, bool cheats[]) = 0;
	virtual void render(SDL_Renderer* const& r, bool const& debug) = 0;
	virtual void reset(SDL_Renderer* const& r) = 0;
protected:
	int state;
};

#endif
