#pragma once

#include "Entity.h"
#include "Collidable.h"
#include "Enum.h"

class Pickup : public Entity, public Collidable
{
public:
	Pickup();
	~Pickup();

	// Entity override
	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	// Collidable override
	int GetType() const override;
	void OnCollision(Collidable* collidable) override;
	const X::Math::Vector2& GetPosition() const override;

	// Pickup Functions
	bool IsActive() const;
	void SetActive(const X::Math::Vector2& position, PickupType type);
	PickupType GetPickupType() const { return mPickupType; }

private:
	bool mIsActive;
	bool mRemoveCollider;
	X::TextureId mImageId;
	X::TextureId mGunImageId;
	X::Math::Vector2 mPosition;
	PickupType mPickupType;
};