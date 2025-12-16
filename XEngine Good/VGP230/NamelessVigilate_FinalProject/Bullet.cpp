#include "Bullet.h"
#include "CollisionManager.h"
#include "Enum.h"

Bullet::Bullet()
    : Entity()
    , Collidable()
    , mImageId(0)
    , mPosition(0.0f, 0.0f)
    , mRotation(0.0f)
    , mLifeTime(0.0f)
    , mRemoveCollider(false)
{
}

Bullet::~Bullet()
{
}

void Bullet::Load()
{
    mImageId = X::LoadTexture("bullet2.png");
    mLifeTime = 0.0f;
    mRemoveCollider = false;
}

void Bullet::Update(float deltaTime)
{
    if (mRemoveCollider)
    {
        CollisionManager::Get()->RemoveCollidable(this);
        mRemoveCollider = false;
    }

    if (!IsActive())
        return;

    mLifeTime -= deltaTime;
    if (mLifeTime <= 0.0f)
    {
        mRemoveCollider = true;
        return;
    }

    const float speed = 400.0f;

    X::Math::Vector2 direction;
    direction.x = cosf(mRotation);
    direction.y = sinf(mRotation);

    mPosition += direction * speed * deltaTime;

    float halfWidth = X::GetSpriteWidth(mImageId) * 0.5f;
    float halfHeight = X::GetSpriteHeight(mImageId) * 0.5f;

    X::Math::Rect rect;
    rect.left = mPosition.x - halfWidth;
    rect.right = mPosition.x + halfWidth;
    rect.top = mPosition.y - halfHeight;
    rect.bottom = mPosition.y + halfHeight;

    SetRect(rect);
}

void Bullet::Render()
{
    if (!IsActive())
        return;

    X::DrawSprite(mImageId, mPosition);
}

void Bullet::Unload()
{
}

void Bullet::SetActive(const X::Math::Vector2& position, float rotation, float lifeTime)
{
    mPosition = position;
    mRotation = rotation;
    mLifeTime = lifeTime;

    float halfWidth = X::GetSpriteWidth(mImageId) * 0.5f;
    float halfHeight = X::GetSpriteHeight(mImageId) * 0.5f;

    X::Math::Rect rect;
    rect.left = mPosition.x - halfWidth;
    rect.right = mPosition.x + halfWidth;
    rect.top = mPosition.y - halfHeight;
    rect.bottom = mPosition.y + halfHeight;

    SetRect(rect);

    SetCollidableFilter(ET_BULLET | ET_ENEMY);
    mRemoveCollider = false;

    CollisionManager::Get()->AddCollidable(this);
}

int Bullet::GetType() const
{
	return IsActive() ? ET_BULLET : ET_NONE;
}

void Bullet::OnCollision(Collidable* collidable)
{
    if (!IsActive())
        return;

    if (collidable->GetType() == ET_ENEMY)
    {
        mLifeTime = 0.0f;
        mRemoveCollider = true;
    }
}