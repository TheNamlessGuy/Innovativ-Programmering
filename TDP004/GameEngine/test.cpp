#include <string>
#include <vector>
#include <iostream>

#include "Engine/StateBasedGame.h"
#include "Engine/GameState.h"

#include "TestState.h"

using namespace std;

int main(int argc, char* argv[])
{
	vector<GameState*> states;
	states.push_back(new TestState());

	StateBasedGame sbg("Test", make_pair(800, 600), states, 0);
	sbg.setTargetDelay(10);
	sbg.setExitOnX(true);

	return sbg.run();
}
