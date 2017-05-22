#ifndef STATEBASEDGAME_H_
#define STATEBASEDGAME_H_

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <map>

#include "InputHandler.h"

class GameState;

class StateBasedGame
{
public:
	StateBasedGame(std::string title,
			std::pair<int, int> screen,
			std::vector<GameState*> states,
			int startingState);
	virtual ~StateBasedGame();

	int run();
	void exit();

	void enableText();

	void toggleFullScreen();
	void setExitOnX(bool b);
	bool isExitingOnX();

	SDL_Renderer* getRenderer();

	void changeState(int newState);
	int getCurrentState();

	bool isMouseButtonDown();
	bool isKeyDown(int keyType);
	void setKeyDown(int keyType, bool isDown);

	bool userTriedToQuit();

	int getMouseX();
	int getMouseY();
	std::pair<int, int> getMousePosition();
	SDL_Rect getMousePosAsRect();

	int getCurrentFPS();
	void setTargetDelay(int newDelay);

	int getWindowWidth();
	int getWindowHeight();
	std::pair<int, int> getWindowSize();
private:
	std::vector<GameState*> states;
	std::pair<int, int> gamescreen;

	int currentState;
	int newState;

	int targetDelay;
	int delay;

	bool running;
	bool triedToQuit;
	bool exitOnX;

	bool mouseDown;
	std::map<int, bool> keysPressed;

	SDL_Window* window;
	SDL_Renderer* renderer;

	InputHandler inputHandler;
};

#endif
