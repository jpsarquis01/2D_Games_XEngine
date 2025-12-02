#pragma once
#include "Entity.h"
#include "Collidable.h"

class Ship;

class PowerUp : public Entity, public Collidable
{
public:
	PowerUp(float radius) : Entity(), Collidable(radius), mActive(false) {}
	~PowerUp() override {}

	void Load() override {}
	void Update(float) override {}
	void Render() override;
	void Unload() override {}

	int GetType() const override { return ET_NONE; }
	const X::Math::Vector2& GetPosition() const override { return mPosition; }
	void OnCollition(Collidable* collidable) override;

	void SpawnRandom();
	void Deactivate() { mActive = false; SetCollitionFilter(ET_NONE); }
	bool IsActive() const { return mActive; }

private:
	X::Math::Vector2 mPosition{ 0.0f, 0.0f };
	bool mActive;
};
