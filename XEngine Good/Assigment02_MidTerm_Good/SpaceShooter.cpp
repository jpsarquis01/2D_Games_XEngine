#include <XEngine.h>
#include "StartState.h"
#include "RunGameState.h"
#include "EndState.h"
#include "Ship.h"

State gActiveState = State::Start;
GameState* gCurrentGameState = nullptr;
std::map<State, GameState*> gGameStates;

//Ship mShip;
void GameInit()
{
	gGameStates[State::Start] = new StartState();
	gGameStates[State::RunGame] = new RunGameState();
	gGameStates[State::End] = new EndState();
	gActiveState = State::Start;

	gCurrentGameState = gGameStates[gActiveState];
	gCurrentGameState->Load();
	//mShip.Load();
}

bool GameLoop(float deltatime)
{
	//mShip.Update(deltatime);
	//mShip.Render();

	State newState = gGameStates[gActiveState]->Update(deltatime);
	gCurrentGameState->Render();
	if (newState != gActiveState)
	{
		gCurrentGameState->Unload();
		gCurrentGameState = gGameStates[newState];
		gCurrentGameState->Load();
		gActiveState = newState;
	}

	// Exit Application
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

void GameCleanup()
{
	//mShip.Unload();

	gCurrentGameState->Unload();
	for (auto& state : gGameStates)
	{
		delete state.second;
	}
	gGameStates.clear();
}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");

	GameInit();

	X::Run(GameLoop);

	GameCleanup();

	X::Stop();

	return 0;
}
