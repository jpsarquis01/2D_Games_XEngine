#pragma once
#include "Entity.h"
#include "Collidable.h"

class Player;

class Enemy : public Entity, public Collidable
{
public:
	Enemy();
	~Enemy();

	// Entity override
	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	// Collidable override
	int GetType() const override;
	void OnCollision(Collidable* collidable) override;
	const X::Math::Vector2& GetPosition() const override;

	// Enemy Functions
	bool IsActive() const;
	void SetActive(const X::Math::Vector2& position, int health, bool isBoss = false);
	void ApplyKnockback(const X::Math::Vector2& direction, float force);
	bool IsBoss() const { return mIsBoss; }
	
	// Player tracking
	void SetPlayerTarget(const X::Math::Vector2& playerPosition);

private:
	X::TextureId mImageId;
	X::TextureId mBossImageId;
	X::Math::Vector2 mPosition;
	X::Math::Vector2 mCenterPoint;
	X::Math::Vector2 mTargetPoint;
	X::Math::Rect mEnemyRect;

	int mHealth;
	float mTargetPointUpdate;
	bool mRemoveCollider;
	
	// Knockback system
	X::Math::Vector2 mKnockbackVelocity;
	float mKnockbackDuration;
	
	// Boss enemy
	bool mIsBoss;
	
	// Player chasing
	X::Math::Vector2 mPlayerPosition;
	const float CHASE_UPDATE_RATE = 0.3f; // Update chase target every 0.3 seconds
};

