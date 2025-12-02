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

	// Ammo/Health getters
	int GetAmmo() const { return mAmmo; }
	bool CanShoot() const { return mAmmo > 0; }
	void ConsumeAmmo() { if (mAmmo > 0) mAmmo--; }
	void AddAmmo(int amount) { mAmmo += amount; }
	int GetHealth() const { return mHealth; }  

private:
	X::TextureId mImageId;
	X::Math::Vector2 mPosition;
	X::Math::Rect mPlayerRect;
	int mHealth;
	bool mRemoveCollider;
	int mAmmo; 
};