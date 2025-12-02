#include "PickupManager.h"
#include "TileMap.h"

PickupManager::PickupManager()
	: Entity()
	, mNextAvailablePickup(0)
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
}

void PickupManager::Update(float deltaTime)
{
	if (X::IsKeyPressed(X::Keys::RSHIFT))
	{
		SpawnPickup(10);
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

void PickupManager::SpawnPickup(int count)
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

	for (int i = 0; i < count; i++)
	{
		prevUsedIndex = mNextAvailablePickup - 1;
		if (prevUsedIndex < 0)
		{
			prevUsedIndex = mPickups.size() - 1;
		}

		while (mNextAvailablePickup)
		{
			Pickup* pickup = mPickups[mNextAvailablePickup];
			mNextAvailablePickup = (mNextAvailablePickup + 1) % mPickups.size();

			if (!pickup->IsActive())
			{
				int randIndex = X::Random(0, walkableTiles.size() - 1);
				Tile* randomTile = walkableTiles[randIndex];
				pickup->SetActive(randomTile->GetPosition());

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


