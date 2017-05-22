#ifndef TESTSTATE_H_
#define TESTSTATE_H_

#include "Engine/StateBasedGame.h"
#include "Engine/GameState.h"

class TestState: public GameState
{
public:
	TestState();
	~TestState();

	void init(StateBasedGame* const& sbg);

	void input(StateBasedGame* const& sbg);
	void update(StateBasedGame* const& sbg);
	void render(StateBasedGame* const& sbg);
};

#endif
