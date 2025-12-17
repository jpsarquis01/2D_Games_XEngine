#pragma once
#include "Entity.h"
#include "Collidable.h"
#include <XEngine.h>

class Bullet : public Entity, public Collidable
{
public:
    Bullet();
    ~Bullet() override;

    // Entity override
    void Load() override;
    void Update(float deltaTime) override;
    void Render() override;
    void Unload() override;

    void SetActive(const X::Math::Vector2& position, float rotation, float lifeTime);

    // Collidable override
    int GetType() const override;
    void OnCollision(Collidable* collidable) override;
    const X::Math::Vector2& GetPosition() const override { return mPosition; }

    bool IsActive() const { return mLifeTime > 0.0f; }

private:
    X::TextureId mImageId;
    X::Math::Vector2 mPosition;
    float mRotation;
    float mLifeTime;
    bool mRemoveCollider;
};