#include "EnemyManager.h"
#include "TileMap.h"

EnemyManager* EnemyManager::mInstance = nullptr;

EnemyManager::EnemyManager()
	: Entity()
	, mNextAvailableEnemy(0)
	, mCurrentWave(0)
	, mWaveActive(false)
	, mWaveStartDelay(0.0f)
{
}

EnemyManager::~EnemyManager()
{
}

EnemyManager* EnemyManager::Get()
{
	if (mInstance == nullptr)
	{
		mInstance = new EnemyManager();
	}
	return mInstance;
}

void EnemyManager::Load()
{
	const int maxEnemies = 30; // Increased for larger waves
	for (int i = 0; i < maxEnemies; ++i)
	{
		Enemy* newEnemy = new Enemy();
		newEnemy->Load();
		mEnemies.push_back(newEnemy);
	}
	mNextAvailableEnemy = 0;
	mCurrentWave = 0;
	mWaveActive = false;
	mWaveStartDelay = 2.0f; // Start first wave after 2 seconds
}

void EnemyManager::Update(float deltaTime)
{
	// Update all enemies
	for (Enemy* enemy : mEnemies)
	{
		enemy->Update(deltaTime);
	}

	// Wave system
	if (!mWaveActive)
	{
		if (mWaveStartDelay > 0.0f)
		{
			mWaveStartDelay -= deltaTime;
			if (mWaveStartDelay <= 0.0f)
			{
				// Start next wave
				mCurrentWave++;
				
				if (mCurrentWave <= 5)
				{
					int enemyCount = 0;
					bool isBoss = false;
					
					switch (mCurrentWave)
					{
					case 1: enemyCount = 3; break;
					case 2: enemyCount = 5; break;
					case 3: enemyCount = 7; break;
					case 4: enemyCount = 9; break;
					case 5: enemyCount = 1; isBoss = true; break; // Final boss
					}
					
					SpawnEnemy(enemyCount, isBoss);
					mWaveActive = true;
				}
			}
		}
	}
	else
	{
		// Check if wave is complete
		if (IsWaveComplete())
		{
			mWaveActive = false;
			if (!AllWavesComplete())
			{
				mWaveStartDelay = WAVE_DELAY;
			}
		}
	}
}

void EnemyManager::Render()
{
	for (Enemy* enemy : mEnemies)
	{
		enemy->Render();
	}
	
	// Display wave information
	if (!AllWavesComplete())
	{
		char waveText[64];
		if (mWaveActive)
		{
			sprintf_s(waveText, "Wave %d / 5", mCurrentWave);
		}
		else if (mWaveStartDelay > 0.0f)
		{
			sprintf_s(waveText, "Next Wave in: %.1f", mWaveStartDelay);
		}
		X::DrawScreenText(waveText, 10.0f, 10.0f, 24.0f, X::Colors::Cyan);
	}
	else
	{
		X::DrawScreenText("ALL WAVES COMPLETE!", 10.0f, 10.0f, 32.0f, X::Colors::Gold);
	}
}

void EnemyManager::Unload()
{
	for (Enemy* enemy : mEnemies)
	{
		enemy->Unload();
		delete enemy;
		enemy = nullptr;
	}
	mEnemies.clear();
}

void EnemyManager::SpawnEnemy(int amount, bool isBoss)
{
	std::vector<Tile*> walkableTiles;
	TileMap::Get()->ObtainAllWalkableTiles(walkableTiles);

	for (Enemy* enemy : mEnemies)
	{
		if (enemy->IsActive())
		{
			for (int i = 0; i < walkableTiles.size(); i++)
			{
				if (walkableTiles[i]->HasCollision(enemy->GetRect()))
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
	
	int spawned = 0;
	int prevUsedIndex = 0;

	for (Enemy* enemy : mEnemies)
	{
		if (spawned >= amount)
			break;

		prevUsedIndex = mNextAvailableEnemy - 1;
		if (prevUsedIndex < 0)
		{
			prevUsedIndex = mEnemies.size() - 1;
		}

		while (mNextAvailableEnemy != prevUsedIndex)
		{
			Enemy* enemy = mEnemies[mNextAvailableEnemy];
			mNextAvailableEnemy = (mNextAvailableEnemy + 1) % mEnemies.size();

			if (!enemy->IsActive())
			{
				int randIndex = X::Random(0, walkableTiles.size() - 1);
				Tile* randomTile = walkableTiles[randIndex];
				
				int health = isBoss ? 200 : 50; // Boss has much more health
				enemy->SetActive(randomTile->GetPosition(), health, isBoss);

				if (randIndex < walkableTiles.size() - 1)
				{
					Tile* tmp = walkableTiles[walkableTiles.size() - 1];
					walkableTiles[walkableTiles.size() - 1] = walkableTiles[randIndex];
					walkableTiles[randIndex] = tmp;
				}

				walkableTiles.pop_back();
				spawned++;
				break;
			}
		}
	}
}

int EnemyManager::GetActiveEnemyCount() const
{
	int count = 0;
	for (Enemy* enemy : mEnemies)
	{
		if (enemy->IsActive())
		{
			count++;
		}
	}
	return count;
}

bool EnemyManager::IsWaveComplete() const
{
	return GetActiveEnemyCount() == 0;
}

void EnemyManager::UpdatePlayerPosition(const X::Math::Vector2& playerPosition)
{
	for (Enemy* enemy : mEnemies)
	{
		if (enemy->IsActive())
		{
			enemy->SetPlayerTarget(playerPosition);
		}
	}
}