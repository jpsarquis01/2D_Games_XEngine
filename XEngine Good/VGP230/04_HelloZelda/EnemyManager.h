#pragma once
#include "Enemy.h"
#include "Entity.h"

class EnemyManager : public Entity
{
public:
	~EnemyManager();
	static EnemyManager* Get();

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	void SpawnEnemy(int amount, bool isBoss = false);
	int GetActiveEnemyCount() const;
	bool IsWaveComplete() const;
	int GetCurrentWave() const { return mCurrentWave; }
	bool AllWavesComplete() const { return mCurrentWave > 5; }
	const std::vector<Enemy*>& GetEnemies() const { return mEnemies; }
	
	// Update all enemies with player position
	void UpdatePlayerPosition(const X::Math::Vector2& playerPosition);

private:
	EnemyManager();
	static EnemyManager* mInstance;

	std::vector<Enemy*> mEnemies;
	int mNextAvailableEnemy;
	
	// Wave system
	int mCurrentWave;
	bool mWaveActive;
	float mWaveStartDelay;
	const float WAVE_DELAY = 3.0f;
};