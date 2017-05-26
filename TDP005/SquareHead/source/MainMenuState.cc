#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "../headers/MainMenuState.h"

using namespace std;

MainMenuState::MainMenuState(SDL_Renderer* const& r, pair<int, int> const& screen_size, TTF_Font* const& font)
	: GameState(GameState::MAINMENU),
	play(screen_size.first/2-50, screen_size.second/2-100, "src/images/buttons/playButton.png", r),
	quit(screen_size.first/2-50, screen_size.second/2+50, "src/images/buttons/quitButton.png", r),
	gamename(new Text(265, 50, "Squarehead", r, font))
{}

MainMenuState::~MainMenuState()
{
	delete gamename;
	gamename = nullptr;
}

void MainMenuState::input(bool KEYS[], bool const& mousedown, SDL_Renderer* const& r)
{
	int mouse_x;
	int mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	play.set_hover(play.is_hover(mouse_x, mouse_y));
	quit.set_hover(quit.is_hover(mouse_x, mouse_y));

	if (mousedown)
	{
		if (play.is_hover(mouse_x, mouse_y))
		{
			state = GameState::PLAY;
		}
		else if (quit.is_hover(mouse_x, mouse_y))
		{
			state = GameState::QUIT;
		}
	}

	if (KEYS[SDLK_ESCAPE])
	{
		state = GameState::QUIT;
	}

	if (KEYS[SDLK_RETURN])
	{
		state = GameState::PLAY;
	}
}

int MainMenuState::update(long long& score, SDL_Renderer* const& r, bool cheats[])
{
	return state;
}

void MainMenuState::render(SDL_Renderer* const& r, bool const& debug)
{
	if (debug)
	{
		SDL_SetRenderDrawColor(r, 50, 50, 200, 255);
		SDL_RenderDrawLine(r, 400, 0, 400, 600);
		SDL_RenderDrawLine(r, 0, 300, 800, 300);
	}

	play.render(r);
	quit.render(r);

	gamename->render(r);
}

void MainMenuState::reset(SDL_Renderer* const& r)
{
	state = GameState::MAINMENU;
}
