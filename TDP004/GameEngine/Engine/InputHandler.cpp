#include <iostream>

#include "InputHandler.h"

using namespace std;

InputHandler::InputHandler() {}

InputHandler::~InputHandler() {}

void InputHandler::handle(SDL_Event e, map<int, bool>& keys,
		bool& mouseDown, bool& triedToQuit)
{
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_KEYDOWN:
			keys[e.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			keys[e.key.keysym.sym] = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouseDown = true;
			break;
		case SDL_MOUSEBUTTONUP:
			mouseDown = false;
			break;
		case SDL_QUIT:
			triedToQuit = true;
			break;
		default:
			break;
		}
	}
}
