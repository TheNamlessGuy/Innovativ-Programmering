#include <iostream>

#include "StateBasedGame.h"
#include "GameState.h"

using namespace std;

StateBasedGame::StateBasedGame(string title, pair<int, int> screen, vector<GameState*> states_i, int startingState)
	:states(states_i), gamescreen(screen),
	 currentState(startingState), newState(startingState),
	 targetDelay(10), delay(0),
	 running(false), triedToQuit(false), exitOnX(false), mouseDown(false)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw;
	}

	window = SDL_CreateWindow(title.c_str(),
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			screen.first, screen.second, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_RenderSetLogicalSize(renderer, screen.first, screen.second);
}

StateBasedGame::~StateBasedGame()
{
	for (GameState* state : states)
	{
		delete state;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

int StateBasedGame::run()
{
	for (GameState* state : states)
	{
		state->init(this);
	}

	int startTime = SDL_GetTicks();
	int lastFrameTime = startTime;

	running = true;

	while(running)
	{
		delay = SDL_GetTicks() - lastFrameTime;
		lastFrameTime += delay;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_Event e;
		inputHandler.handle(e, keysPressed, mouseDown, triedToQuit);

		if (triedToQuit && exitOnX)
		{
			exit();
		}

		states.at(currentState)->input(this);

		states.at(currentState)->update(this);

		states.at(currentState)->render(this);
		SDL_RenderPresent(renderer);

		currentState = newState;

		delay = SDL_GetTicks() - lastFrameTime;

		if (targetDelay > delay)
		{
			SDL_Delay(targetDelay - delay);
		}
	}

	return 0;
}

void StateBasedGame::enableText()
{

}

void StateBasedGame::changeState(int newState_i)
{
	newState = newState_i;
}

int StateBasedGame::getCurrentState()
{
	return currentState;
}

SDL_Renderer* StateBasedGame::getRenderer()
{
	return renderer;
}

void StateBasedGame::exit()
{
	running = false;
}

bool StateBasedGame::isMouseButtonDown()
{
	return mouseDown;
}

bool StateBasedGame::isKeyDown(int keyType)
{
	return keysPressed[keyType];
}

void StateBasedGame::setKeyDown(int keyType, bool isDown)
{
	keysPressed[keyType] = isDown;
}

bool StateBasedGame::userTriedToQuit()
{
	return triedToQuit;
}

int StateBasedGame::getMouseX()
{
	return getMousePosition().first;
}

int StateBasedGame::getMouseY()
{
	return getMousePosition().second;
}

pair<int, int> StateBasedGame::getMousePosition()
{
	pair<int, int> return_pair;
	SDL_GetMouseState(&return_pair.first, &return_pair.second);
	return return_pair;
}

SDL_Rect StateBasedGame::getMousePosAsRect()
{
	SDL_Rect temp;
	temp.x = getMouseX();
	temp.y = getMouseY();
	temp.w = 1;
	temp.h = 1;

	return temp;
}

int StateBasedGame::getCurrentFPS()
{
	return (1000 / delay);
}

void StateBasedGame::setTargetDelay(int newDelay)
{
	newDelay = targetDelay;
}

void StateBasedGame::setExitOnX(bool b)
{
	exitOnX = b;
}

bool StateBasedGame::isExitingOnX()
{
	return exitOnX;
}

int StateBasedGame::getWindowWidth()
{
	return gamescreen.first;
}
int StateBasedGame::getWindowHeight()
{
	return gamescreen.second;
}
std::pair<int, int> StateBasedGame::getWindowSize()
{
	return gamescreen;
}
