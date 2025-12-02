#pragma once
#include "Entity.h"
#include "Collidable.h"

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
	void SetActive(const X::Math::Vector2& position, int health);

private:
	X::TextureId mImageId;
	X::Math::Vector2 mPosition;
	X::Math::Vector2 mCenterPoint;
	X::Math::Vector2 mTargetPoint;
	X::Math::Rect mEnemyRect;

	int mHealth;
	float mTargetPointUpdate;
	bool mRemoveCollider;
};