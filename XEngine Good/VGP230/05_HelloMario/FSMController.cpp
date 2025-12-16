#include "FSMController.h"
#include "States.h"

FSMController::FSMController()
	: mCurrentState(MS_NONE)
{
}

FSMController::~FSMController()
{
	
}

void FSMController::AddState(MovementState stateType, State* state)
{
	XASSERT(mStates[moveState] == nullptr, "State already exists!");
	mStates[moveState] = state;
}

void FSMController::SetState(MovementState stateType)
{
	if (mCurrentState != moveState)
	{
		if (mStates[mCurrentState] != nullptr)
		{
			mStates[mCurrentState]->OnExitState();
		}

		mCurrentState = moveState;

		if (mStates[mCurrentState] != nullptr)
		{
			mStates[mCurrentState]->OnEnterState();
		}
	}
}

void FSMController::Cleanup()
{

}