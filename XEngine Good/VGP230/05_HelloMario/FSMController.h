#pragma once
#include "Enum.h"
#include <map>

class State;

class FSMController
{
public:
	FSMController();
	~FSMController();

	void AddState(MovementState stateType, State* state);
	void SetState(MovementState stateType);
	void Update(float deltaTime);
	void Cleanup();

private:
	MovementState mCurrentState;
	std::map<MovementState, State*> mStates;
};