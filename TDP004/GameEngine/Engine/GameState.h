#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "StateBasedGame.h"

class GameState
{
public:
	GameState();
	virtual ~GameState();

	virtual void init(StateBasedGame* const& sbg) = 0;

	virtual void input(StateBasedGame* const& sbg) = 0;
	virtual void update(StateBasedGame* const& sbg) = 0;
	virtual void render(StateBasedGame* const& sbg) = 0;
};

#endif
