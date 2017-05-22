#ifndef ENGINE_INPUTHANDLER_H_
#define ENGINE_INPUTHANDLER_H_

#include <SDL2/SDL.h>
#include <map>

class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	void handle(SDL_Event e, std::map<int, bool>& keys,
			bool& mouseDown, bool& triedToQuit);
};

#endif
