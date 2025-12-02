#pragma once

#include "Entity.h"
#include "Collidable.h"


class BulletPool;
class Ship;
class AnimSpriteSheet;

class Enemy : public Entity, public Collidable
{
public:
	Enemy();
	~Enemy() override;

	// Entity overrides
	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	// Collidable overrides
	int GetType() const override;
	const X::Math::Vector2& GetPosition() const override;
	void OnCollition(Collidable* collidable) override;

	//Behavior Functions
	void SetBulletPool(BulletPool* bulletPool);
	void SetShip(Ship* ship);
	void SetPosition(const X::Math::Vector2& position);
	void SetRotation(float rotation);

	bool IsAlive() const;

private:
	BulletPool* mBulletPool;
	Ship* mShip;
	AnimSpriteSheet* mExplosionAnim;

	X::TextureId mImageId;
	X::Math::Vector2 mPosition;
	float mRotation;

	int mHealth;

	X::Math::Vector2 mCenterPoint;
	X::Math::Vector2 mTargetPosition;
	float mFireRate;
	float mTargetPositionUpdate;
};