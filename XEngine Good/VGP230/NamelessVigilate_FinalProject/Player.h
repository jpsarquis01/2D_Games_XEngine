#pragma once

#include "Entity.h"
#include "Collidable.h"
#include "Enum.h"

class Player : public Entity, public Collidable
{
public:
	Player();
	~Player();

	// Entity override
	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	// Collidable override
	int GetType() const override;
	void OnCollision(Collidable* collidable) override;
	const X::Math::Vector2& GetPosition() const override;

	// Ammo/Health getters
	int GetAmmo() const { return mAmmo; }
	bool CanShoot() const { return mAmmo > 0; }
	void ConsumeAmmo() { if (mAmmo > 0) mAmmo--; }
	void AddAmmo(int amount) { mAmmo += amount; }
	int GetHealth() const { return mHealth; }
	
	// Weapon system
	WeaponType GetCurrentWeapon() const { return mCurrentWeapon; }
	void SetWeapon(WeaponType weapon) { mCurrentWeapon = weapon; }
	bool HasGun() const { return mHasGun; }
	void GiveGun() { mHasGun = true; mCurrentWeapon = WT_GUN; }
	
	// Punch system
	void PerformPunch();
	bool IsPunching() const { return mPunchActive; }
	const X::Math::Rect& GetPunchRect() const { return mPunchRect; }
	float GetPunchDirection() const { return mPunchDirection; }

private:
	X::TextureId mImageId;
	X::Math::Vector2 mPosition;
	X::Math::Rect mPlayerRect;
	int mHealth;
	bool mRemoveCollider;
	int mAmmo;                  // Ammo count
	
	// Weapon system
	WeaponType mCurrentWeapon;  // Current equipped weapon
	bool mHasGun;               // Has the player picked up the gun?

	// Press '1' -> Switch to Punch (always works)
	// Press '2' -> Check if has gun -> YES : Switch to Gun(if ammo > 0) -> NO : Stay on current weapon
	
	// Punch system
	bool mPunchActive;
	float mPunchTimer;
	float mPunchCooldown;
	X::Math::Rect mPunchRect;
	float mPunchDirection;
	const float PUNCH_DURATION = 0.2f;
	const float PUNCH_COOLDOWN = 0.5f;
	const float PUNCH_RANGE = 40.0f;
};