#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "../headers/SquareHead.h"
#include "../headers/GameState.h"
#include "../headers/MainMenuState.h"
#include "../headers/PlayState.h"
#include "../headers/GameOverState.h"

using namespace std;

const pair<int, int> SquareHead::SCREEN_SIZE{800, 600};

SquareHead::SquareHead(): cheat_string(""), current_state(GameState::MAINMENU), new_state(GameState::MAINMENU),
		target_frame_delay(10), mousedown(false), quit_by_x(false)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		cerr << "Error initializing SDL" << endl;
		exit(1);
	}

	if (TTF_Init() != 0)
	{
		cerr << "Error initalizing TTF" << endl;
		exit(1);
	}

	for (bool& b : KEYS)
	{
		b = false;
	}

	for (bool& b : cheats)
	{
		b = false;
	}

	title_font = TTF_OpenFont("src/fonts/BEBAS.ttf", 50);
	font = TTF_OpenFont("src/fonts/BEBAS.ttf", 30);
}

SquareHead::~SquareHead() {}

void SquareHead::user_input(SDL_Event e)
{
	while (SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_KEYDOWN:
			if(e.key.keysym.sym <= 322){
				cheat_string += char(e.key.keysym.sym);
				KEYS[e.key.keysym.sym] = true;
			}
			break;
		case SDL_KEYUP:
			if(e.key.keysym.sym <= 322){
				KEYS[e.key.keysym.sym] = false;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			mousedown = true;
			break;
		case SDL_MOUSEBUTTONUP:
			mousedown = false;
			break;
		case SDL_QUIT:
			quit_by_x = true;
			break;
		default:
			break;
		}
	}
}

void SquareHead::check_cheats()
{
	if (cheat_string.find("debug") != string::npos)
	{
		cheat_string = "";
		cheats[SquareHead::CHEAT_DEBUG] = !cheats[SquareHead::CHEAT_DEBUG];
	}

	if (cheat_string.find("assassinscreed") != string::npos)
	{
		if (!cheats[SquareHead::CHEAT_FRAMEDROP])
		{
			target_frame_delay = 100;
			cheats[SquareHead::CHEAT_FRAMEDROP] = true;
		}
		else
		{
			target_frame_delay = 10;
			cheats[SquareHead::CHEAT_FRAMEDROP] = false;
		}
		cheat_string = "";
	}

	if (cheat_string.find("bulletproof") != string::npos)
	{
		cheats[SquareHead::CHEAT_IMMORTAL] = !cheats[SquareHead::CHEAT_IMMORTAL];
		cheat_string = "";
	}

	if (cheat_string.size() > 100)
	{
		cheat_string = "";
	}
}

int SquareHead::run()
{
	SDL_Window* window = SDL_CreateWindow("SquareHead", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_SIZE.first, SCREEN_SIZE.second, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_SIZE.first, SCREEN_SIZE.second);

	vector<GameState*> states {new MainMenuState(renderer, SCREEN_SIZE, title_font),
			new PlayState(renderer, font),
			new GameOverState(renderer, font)};

	long long score = 0;

	int starttime = SDL_GetTicks();
	int last_frame_time = starttime;

	while (current_state != GameState::QUIT)
	{
		//TIMING
		int frame_delay = SDL_GetTicks() - last_frame_time;
		last_frame_time += frame_delay;

		//Input
		SDL_Event e;
		user_input(e);
		check_cheats();

		states[current_state]->input(KEYS, mousedown, renderer);

		//Update
		new_state = states[current_state]->update(score, renderer, cheats);

		//Clear Screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//Draw state
		states[current_state]->render(renderer, cheats[SquareHead::CHEAT_DEBUG]);
		SDL_RenderPresent(renderer);

		//FPS
		frame_delay = SDL_GetTicks() - last_frame_time;

		if (target_frame_delay > frame_delay)
		{
			SDL_Delay(target_frame_delay - frame_delay);
		}

		//Reset current state if switching state
		if (new_state != current_state)
		{
			states[current_state]->reset(renderer);
			current_state = new_state;
		}

		//If X in top right corner is pressed
		if (quit_by_x)
		{
			current_state = GameState::QUIT;
		}
	}

	for (GameState* state: states)
	{
		delete state;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_CloseFont(title_font);
	TTF_CloseFont(font);

	TTF_Quit();
	SDL_Quit();

	return 0;
}
