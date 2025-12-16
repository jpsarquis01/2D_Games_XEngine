#include "PickupManager.h"
#include "TileMap.h"

PickupManager* PickupManager::mInstance = nullptr;

PickupManager::PickupManager()
	: Entity()
	, mNextAvailablePickup(0)
	, mSpawnTimer(0.0f)
	, mNextSpawnTime(0.0f)
{
}

PickupManager::~PickupManager()
{
}

PickupManager* PickupManager::Get()
{
	if (mInstance == nullptr)
	{
		mInstance = new PickupManager();
	}
	return mInstance;
}

void PickupManager::Load()
{
	const int maxPickups = 100;

	for (int i = 0; i < maxPickups; ++i)
	{
		Pickup* pickup = new Pickup();
		pickup->Load();
		mPickups.push_back(pickup);
	}

	mNextAvailablePickup = 0;
	
	// Initialize random spawn timer
	mSpawnTimer = 0.0f;
	mNextSpawnTime = X::RandomFloat(MIN_SPAWN_TIME, MAX_SPAWN_TIME);
}

void PickupManager::Update(float deltaTime)
{
	// Random ammo spawning system
	mSpawnTimer += deltaTime;
	if (mSpawnTimer >= mNextSpawnTime)
	{
		// Spawn 1-3 ammo pickups randomly
		int ammoCount = X::Random(1, 3);
		SpawnPickup(ammoCount, PT_AMMO);
		
		// Reset timer with new random interval
		mSpawnTimer = 0.0f;
		mNextSpawnTime = X::RandomFloat(MIN_SPAWN_TIME, MAX_SPAWN_TIME);
	}

	for (Pickup* pickup : mPickups)
	{
		pickup->Update(deltaTime);
	}
}

void PickupManager::Render()
{
	for (Pickup* pickup : mPickups)
	{
		pickup->Render();
	}
}

void PickupManager::Unload()
{
	for (Pickup* pickup : mPickups)
	{
		pickup->Unload();
		delete pickup;
		pickup = nullptr;
	}

	mPickups.clear();
}

void PickupManager::SpawnPickup(int amount, PickupType type)
{
	std::vector<Tile*> walkableTiles;
	TileMap::Get()->ObtainAllWalkableTiles(walkableTiles);

	for (Pickup* pickup : mPickups)
	{
		if (pickup->IsActive())
		{
			for (int i = 0; i < walkableTiles.size(); i++)
			{
				if (walkableTiles[i]->HasCollision(pickup->GetRect()))
				{
					if (i < walkableTiles.size() - 1)
					{
						Tile* tmp = walkableTiles[walkableTiles.size() - 1];
						walkableTiles[walkableTiles.size() - 1] = walkableTiles[i];
						walkableTiles[i] = tmp;
					}
					walkableTiles.pop_back();
					break;
				}
			}
		}
	}
	int prevUsedIndex = 0;

	for (int i = 0; i < amount; i++)
	{
		prevUsedIndex = mNextAvailablePickup - 1;
		if (prevUsedIndex < 0)
		{
			prevUsedIndex = mPickups.size() - 1;
		}

		while (mNextAvailablePickup != prevUsedIndex)
		{
			Pickup* pickup = mPickups[mNextAvailablePickup];
			mNextAvailablePickup = (mNextAvailablePickup + 1) % mPickups.size();

			if (!pickup->IsActive())
			{
				int randIndex = X::Random(0, walkableTiles.size() - 1);
				Tile* randomTile = walkableTiles[randIndex];
				pickup->SetActive(randomTile->GetPosition(), type);

				if (randIndex < walkableTiles.size() - 1)
				{
					Tile* tmp = walkableTiles[walkableTiles.size() - 1];
					walkableTiles[walkableTiles.size() - 1] = walkableTiles[randIndex];
					walkableTiles[randIndex] = tmp;
				}

				walkableTiles.pop_back();
				break;
			}
		}
	}
}


