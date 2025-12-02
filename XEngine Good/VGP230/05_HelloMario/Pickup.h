#pragma once

#include "Entity.h"
#include "Collidable.h"

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
	void SetActive(const X::Math::Vector2& position);

private:
	bool IsActive;
	bool mRemoveCollider;
	X::TextureId mImageId;
	X::Math::Vector2 mPosition;

	int mType;
};