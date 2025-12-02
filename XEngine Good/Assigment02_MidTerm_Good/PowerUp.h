#pragma once
#include "Entity.h"
#include "Collidable.h"

class Ship;

class PowerUp : public Entity, public Collidable
{
public:
	PowerUp(float radius) : Entity(), Collidable(radius), mActive(false) {} // radius is used for collision size
	~PowerUp() override {}

	void Load() override {}
	void Update(float) override {}
	void Render() override;
	void Unload() override {}

	int GetType() const override { return ET_NONE; } // powerups donr behave like enemies so return ET_NONE
	const X::Math::Vector2& GetPosition() const override { return mPosition; } // return position for collision detection
	void OnCollition(Collidable* collidable) override; // Called when ship collides with powerup

	void SpawnRandom();
	void Deactivate() { mActive = false; SetCollitionFilter(ET_NONE); } // Deactivate powerup and disable collision
	bool IsActive() const { return mActive; } // Check if powerup is active

private:
	X::Math::Vector2 mPosition{ 0.0f, 0.0f };
	bool mActive; 
};
