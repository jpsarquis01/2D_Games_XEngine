#pragma once
#include "Entity.h"
#include "Collidable.h"

class BulletPool;

class Ship : public Entity, public Collidable
{
public:
	Ship();
	~Ship() override;

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	int GetType() const override;
	const X::Math::Vector2& GetPosition() const override;
	void OnCollition(Collidable* collidable) override;

	void SetBulletPool(BulletPool* bulletPool);

	int GetHealth() const;
	int GetMaxHealth() const;
	bool IsAlive() const;

	// PowerUps
	void ApplyShield(float seconsds); // Called by ShieldPowerUp.cpp to start shield
	float GetShieldTime() const { return mShieldTime; } // Called by Game.cpp to check shield time in HUD

	void AddDash(int n) { mDash += n; } // Called by DashPowerUp.cpp to add dash count
	int GetDash() const { return mDash; } // Called by Game.cpp to get dash count in HUD

	void DoNovaShot(); // Called by NovaPowerUp.cpp to do nova shot 

private:
	X::TextureId mImageId; 
	X::Math::Vector2 mPosition;
	float mRotation;
	float scale;
	int mHealth;
	int mMaxHealth;

	// Extra Weapons
	enum class WeaponType // Called by Game.cpp to switch weapon type
	{
		SingleShot,
		TripleShot
	};
	WeaponType mWeapon = WeaponType::SingleShot;

	float mFireCooldown = 0.15f; // Time between shots
	float mFireTimer = 0.0f; // Timer to track time since last shot

	// PowerUps
	float mShieldTime = 0.0f; // Shield duration
	int mDash = 0; // Dash count
	float mDashTime = 0.0f; // Dash duration
	float mDashSpeedMul = 3.0f; // Dash speed multiplier

	// Pool that owns all bullet obj
	BulletPool* mBulletPool;
};