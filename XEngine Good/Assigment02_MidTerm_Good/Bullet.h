#pragma once
#include "Entity.h"
#include "Collidable.h"

class Bullet : public Entity, public Collidable
{
public:
	Bullet();
	~Bullet() override;

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	void SetActive(const X::Math::Vector2& pos, float rotation, float lifeTime);
	void SetEntityType(EntityType type);
	bool IsActive() const;

	int GetType() const override;
	const X::Math::Vector2& GetPosition() const override;
	void OnCollition(Collidable* collidable) override;

private:
	X::TextureId mImageId;
	X::Math::Vector2 mPosition;
	float mRotation;
	float mlifeTime;
	EntityType mBulletType;
};
