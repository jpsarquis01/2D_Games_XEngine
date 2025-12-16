#pragma once
#include "Entity.h"
#include "Collidable.h"

class Projectile : public Entity, public Collidable
{
public:
	Projectile();
	~Projectile();

	// Entity override
	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	// Collidable override
	int GetType() const override;
	void OnCollision(Collidable* collidable) override;
	const X::Math::Vector2& GetPosition() const override;

	// Projectile Functions
	void SetActive(const X::Math::Vector2& position, const X::Math::Vector2& direction, float lifetime = 2.0f);
	void SetRotation(float rotation);
	void SetFlip(X::Flip flip);
	bool IsActive() const;

private:
	X::TextureId mImageId;
	X::Math::Vector2 mPosition;
	X::Math::Rect mProjectileRect;
	X::Flip mFlip;
	float mLifeTime;
	EntityType mBulletType;
};