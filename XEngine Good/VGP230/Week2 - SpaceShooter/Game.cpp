#include "Game.h"
#include "Ship.h"
#include "BulletPool.h"
#include "Bullet.h"
#include "Enemy.h"
#include "ProgressBar.h"
#include "ShieldPowerUp.h"
#include "DashPowerUp.h"
#include "NovaPowerUp.h"

Game::Game()
	:Entity()
	, mPlayer(nullptr)
	, mBulletPool(nullptr)
	, mHealthBar(nullptr)
{

}

Game::~Game()
{
	XASSERT(mPlayer == nullptr, "Game: Unload must be called");
}

void Game::Load()
{
	mPlayer = new Ship();
	mBulletPool = new BulletPool();
	mHealthBar = new ProgressBar();

	// PowerUps
	mShieldPowerUp = new ShieldPowerUp();
	mDashPowerUp = new DashPowerUp();
	mNovaPowerUp = new NovaPowerUp();

	mPlayer->Load();
	mPlayer->SetBulletPool(mBulletPool);
	AddCollidable(mPlayer);

	mHealthBar->Load();

	mShieldPowerUp->Load();
	mDashPowerUp->Load();
	mNovaPowerUp->Load();

	mBulletPool->Load();
	std::vector<Bullet*>& bullets = mBulletPool->GetBullets();
	for (Bullet* bullet : bullets)
	{
		AddCollidable(bullet);
	}

	// PowerUps
	AddCollidable(mShieldPowerUp);
	AddCollidable(mDashPowerUp);
	AddCollidable(mNovaPowerUp);

	// First spawn times
	mShieldSpawn = 3.0f;
	mDashSpawn = 5.0f;
	mNovaSpawn = 10.0f;

	// Waves
	mCurrentWave = 1;
	SpawnWave(mCurrentWave);

}

void Game::Update(float deltaTime)
{
	mPlayer->Update(deltaTime);

	for (Enemy* enemy : mEnemies)
	{
		enemy->Update(deltaTime);
	}

	mBulletPool->Update(deltaTime);

	// PowerUps
	mShieldSpawn -= deltaTime;
	mDashSpawn -= deltaTime; // Fix
	mNovaSpawn -= deltaTime;

	if (mShieldSpawn <= 0.0f && mShieldPowerUp && !mShieldPowerUp->IsActive())
	{
		mShieldPowerUp->SpawnRandom();
		mShieldSpawn = X::RandomFloat(10.0f, 20.0f);
	}

	if (mDashSpawn <= 0.0f && mDashPowerUp && !mDashPowerUp->IsActive())
	{
		mDashPowerUp->SpawnRandom();
		mDashSpawn = X::RandomFloat(10.0f, 20.0f);
	}

	if (mNovaSpawn <= 0.0f && mNovaPowerUp && !mNovaPowerUp->IsActive())
	{
		mNovaPowerUp->SpawnRandom();
		mNovaSpawn = X::RandomFloat(10.0f, 20.0f);
	}

	int numCollidable = mCollidables.size();
	for (int i = 0; i < numCollidable - 1; ++i)
	{
		for (int n = i + 1; n < numCollidable; ++n)
		{
			if (mCollidables[i]->DiCollide(mCollidables[n]))
			{
				mCollidables[i]->OnCollition(mCollidables[n]);
				mCollidables[n]->OnCollition(mCollidables[i]);
			}
		}
	}

	// Waves
	if (!mWaitingForNextWave)
	{
		if (AliveEnemyCount() == 0)
		{
			mWaitingForNextWave = true;
			mInterWaveTimer = 0.0f;
		}
	}
	else
	{
		mInterWaveTimer += deltaTime;

		if (mInterWaveTimer >= 2.0f)
		{
			++mCurrentWave;
			if (mCurrentWave <= mTotalWave)
			{
				ClearDeadEnemies();
				SpawnWave(mCurrentWave);
			}
		}
	}

	mHealthBar->SetBarValue(mPlayer->GetHealth(), mPlayer->GetMaxHealth());
}

void Game::Render()
{
	mPlayer->Render();
	for (Enemy* enemy : mEnemies)
	{
		enemy->Render();
	}
	mBulletPool->Render();
	mHealthBar->Render();

	// PowerUps
	if(mShieldPowerUp)
	{
		mShieldPowerUp->Render();
	}
	if (mDashPowerUp)
	{
		mDashPowerUp->Render();
	}
	if (mNovaPowerUp)
	{
		mNovaPowerUp->Render();
	}

	// Waves HUD
	char waveText[64];
	if (mCurrentWave <= mTotalWave)
	{
		sprintf_s(waveText, "Wave %d / %d", mCurrentWave, mTotalWave);
	}
	else
	{
		sprintf_s(waveText, "All Waves Compleated");
	}

	float size = 36.0f;
	float textWidth = X::GetTextWidth(waveText, size);
	float x = (X::GetScreenWidth() - textWidth) * 0.5f;
	float y = 20.0f;

	X::DrawScreenText(waveText, x, y, size, X::Colors::White);

	// PowerUps HUD
	char hud[128];
	// Shield
	if(mPlayer->GetShieldTime() > 0.0f)
	{
		sprintf_s(hud, "Shield: %.1f", mPlayer->GetShieldTime());
		float tw = X::GetTextWidth(hud, 28.0f);
		X::DrawScreenText(hud, X::GetScreenWidth() - tw - 20.0f, 20.0f, 28.0f, X::Colors::Cyan);
	}
	// Dash
	sprintf_s(hud, "Dash: %d", mPlayer->GetDash());
	X::DrawScreenText(hud, 20.0f, 60.0f, 28.0f, X::Colors::Yellow);
}

int Game::AliveEnemyCount() const
{
	int alive = 0;
	for(const Enemy* enemy : mEnemies)
	{
		if(enemy->IsAlive())
		{
			++alive;
		}
	}
	return alive;
}

void Game::ClearDeadEnemies()
{

}

void Game::SpawnWave(int waveIndex)
{
	static const int eCount[] = { 3, 5, 7, 10, 15 };
	int count = eCount[std::min(waveIndex, (int)(sizeof(eCount) / sizeof(eCount[0]))) - 1];

	X::Math::Vector2 spawnPos = X::Math::Vector2::Zero();
	X::Math::Vector2 spawnDirection = X::Math::Vector2::Zero();
	X::Math::Vector2 center = { X::GetScreenWidth() * 0.5f, X::GetScreenHeight() * 0.5f };
	const float minOffset = 100.0f;
	const float maxOffset = center.y;

	for (int i = 0; i < count; ++i)
	{
		spawnDirection = X::RandomUnitCircle();
		spawnPos = center + (spawnDirection * X::RandomFloat(minOffset, maxOffset));

		Enemy* newEnemy = new Enemy();
		newEnemy->Load();
		newEnemy->SetBulletPool(mBulletPool);
		newEnemy->SetShip(mPlayer);
		newEnemy->SetPosition(spawnPos);
		newEnemy->SetRotation(X::RandomFloat() * X::Math::kTwoPi);
		
		AddCollidable(newEnemy);
		mEnemies.push_back(newEnemy);
	}
	mWaitingForNextWave = false;
	mInterWaveTimer = 0.0f;
}

void Game::Unload()
{
	mBulletPool->Unload();
	delete mBulletPool;
	mBulletPool = nullptr;


	mHealthBar->Unload();
	delete mHealthBar;
	mHealthBar = nullptr;

	mPlayer->Unload();
	delete mPlayer;
	mPlayer = nullptr;

	for (Enemy* enemy : mEnemies)
	{
		enemy->Unload();
		delete enemy;
		enemy = nullptr;
	}
	mEnemies.clear();

	// PowerUps
	if(mShieldPowerUp)
	{
		mShieldPowerUp->Unload();
		delete mShieldPowerUp;
		mShieldPowerUp = nullptr;
	}
	if (mDashPowerUp)
	{
		mDashPowerUp->Unload();
		delete mDashPowerUp;
		mDashPowerUp = nullptr;
	}
	if (mNovaPowerUp)
	{
		mNovaPowerUp->Unload();
		delete mNovaPowerUp;
		mNovaPowerUp = nullptr;
	}
}

void Game::AddCollidable(Collidable* collidable)
{
	mCollidables.push_back(collidable);
}

bool Game::IsGameOver()
{
	return !mPlayer->IsAlive();
}

