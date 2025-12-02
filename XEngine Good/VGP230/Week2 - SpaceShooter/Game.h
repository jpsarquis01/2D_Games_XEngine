#pragma once
#include "Entity.h"
#include <vector>

class Ship;
class BulletPool;
class Collidable;
class Enemy;
class ProgressBar;

// PowerUps
class PowerUp;
class ShieldPowerUp;
class DashPowerUp;
class NovaPowerUp;

class Game : public Entity
{
public:
	Game();
	~Game() override;

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	void AddCollidable(Collidable* collidable);

	bool IsGameOver();

	// Helpers Waves
	void SpawnWave(int waveIndex);
	int AliveEnemyCount() const;
	void ClearDeadEnemies();

private:
	Ship* mPlayer;
	BulletPool* mBulletPool;
	std::vector<Collidable*> mCollidables;

	std::vector<Enemy*> mEnemies;
	ProgressBar* mHealthBar;

	ShieldPowerUp* mShieldPowerUp = nullptr;
	DashPowerUp* mDashPowerUp = nullptr;
	NovaPowerUp* mNovaPowerUp = nullptr;

	// PowerUp spawn timer
	float mShieldSpawn = 0.0f;
	float mDashSpawn = 0.0f;
	float mNovaSpawn = 0.0f;

	// Enemy Waves
	int mCurrentWave = 0;
	const int mTotalWave = 5;
	float mInterWaveTimer = 0.0f;
	bool mWaitingForNextWave = false;
};