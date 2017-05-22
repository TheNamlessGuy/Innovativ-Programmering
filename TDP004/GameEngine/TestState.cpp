#include <iostream>

#include "TestState.h"

using namespace std;

TestState::TestState()
{}

TestState::~TestState()
{}

void TestState::init(StateBasedGame* const& sbg)
{
	cout << "INIT" << endl;
}

void TestState::input(StateBasedGame* const& sbg)
{
	cout << "INPUT" << endl;
}

void TestState::update(StateBasedGame* const& sbg)
{
	cout << "UPDATE" << endl;
}

void TestState::render(StateBasedGame* const& sbg)
{
	cout << "RENDER" << endl;
}
