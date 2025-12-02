#include "GameController.h"
#include "TileMap.h"
#include "CollisionManager.h"
#include "PickupManager.h"
#include "EnemyManager.h"

GameController* GameController::mInstance = nullptr;

GameController::GameController()
	: Entity()
{
}

GameController::~GameController()
{
}

GameController* GameController::Get()
{
	if (mInstance == nullptr)
	{
		mInstance = new GameController();
	}
	return mInstance;
}

// Entity override
void GameController::Load()
{
	CollisionManager::Get()->Load();
	TileMap::Get()->Load();
	PickupManager::Get()->Load();
	EnemyManager::Get()->Load();
	mPlayer.Load();
}

void GameController::Update(float deltaTime)
{
	TileMap::Get()->Update(deltaTime);
	mPlayer.Update(deltaTime);
	EnemyManager::Get()->Update(deltaTime);
	CollisionManager::Get()->Update(deltaTime);
	PickupManager::Get()->Update(deltaTime);
}

void GameController::Render()
{
	TileMap::Get()->Render();
	PickupManager::Get()->Render();
	mPlayer.Render();
	EnemyManager::Get()->Render();

	// Only for debugging
	CollisionManager::Get()->Render();
}

void GameController::Unload()
{
	mPlayer.Unload();
	EnemyManager::Get()->Unload();
	TileMap::Get()->Unload();
	PickupManager::Get()->Unload();
	CollisionManager::Get()->Unload();
}