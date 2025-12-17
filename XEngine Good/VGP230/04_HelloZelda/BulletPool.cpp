#include "BulletPool.h"
#include "Bullet.h"

BulletPool::BulletPool()
    : Entity()
    , mNextAvailableIndex(0)
{
}

BulletPool::~BulletPool()
{
}

void BulletPool::Load()
{
    const int maxBullets = 100;

    for (int i = 0; i < maxBullets; ++i)
    {
        Bullet* bullet = new Bullet();
        bullet->Load();
        mBullets.push_back(bullet);
    }

    mNextAvailableIndex = 0;
}

void BulletPool::Update(float deltaTime)
{
    for (Bullet* bullet : mBullets)
    {
        bullet->Update(deltaTime);
    }
}

void BulletPool::Render()
{
    for (Bullet* bullet : mBullets)
    {
        bullet->Render();
    }
}

void BulletPool::Unload()
{
    for (Bullet* bullet : mBullets)
    {
        bullet->Unload();
        delete bullet;
    }
    mBullets.clear();
}

Bullet* BulletPool::GetBullet()
{
    if (mBullets.empty())
        return nullptr;

    int startIndex = mNextAvailableIndex;

    do
    {
        Bullet* bullet = mBullets[mNextAvailableIndex];
        mNextAvailableIndex = (mNextAvailableIndex + 1) % mBullets.size();

        if (!bullet->IsActive())
            return bullet;

    } while (mNextAvailableIndex != startIndex);

    return mBullets[mNextAvailableIndex];
}