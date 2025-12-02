#pragma once

#include "Entity.h"
#include "Collidable.h"

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

	// Player Functions
	void SetPosition(const X::Math::Vector2& position);
	void SetVelocity(const X::Math::Vector2& velocity);
	const X::Math::Vector2& GetVelocity() const;

private:
	X::TextureId mImageId;
	X::Math::Vector2 mPosition;
	X::Math::Vector2 mVelocity;
	X::Math::Rect mPlayerRect;
	int mHealth;
	bool mRemoveCollider;
};