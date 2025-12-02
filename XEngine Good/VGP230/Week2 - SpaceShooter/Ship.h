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
	void ApplyShield(float seconsds);
	float GetShieldTime() const { return mShieldTime; }

	void AddDash(int n) { mDash += n; }
	int GetDash() const { return mDash; }

	void DoNovaShot();

private:
	X::TextureId mImageId;
	X::Math::Vector2 mPosition;
	float mRotation;
	float scale;
	int mHealth;
	int mMaxHealth;

	// Extra Weapons
	enum class WeaponType
	{
		SingleShot,
		TripleShot
	};
	WeaponType mWeapon = WeaponType::SingleShot;
	float mFireCooldown = 0.15f;
	float mFireTimer = 0.0f;

	// PowerUps
	float mShieldTime = 0.0f;
	int mDash = 0;
	float mDashTime = 0.0f;
	float mDashSpeedMul = 3.0f;


	BulletPool* mBulletPool;
};