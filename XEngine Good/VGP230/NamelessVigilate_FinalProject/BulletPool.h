#pragma once
#include "Entity.h"
#include <vector>

class Bullet;

class BulletPool : public Entity
{
public:
    BulletPool();
    ~BulletPool() override;

    void Load() override;
    void Update(float deltaTime) override;
    void Render() override;
    void Unload() override;

    Bullet* GetBullet();
    const std::vector<Bullet*>& GetBullets() const { return mBullets; }

private:
    int mNextAvailableIndex;
    std::vector<Bullet*> mBullets;
};