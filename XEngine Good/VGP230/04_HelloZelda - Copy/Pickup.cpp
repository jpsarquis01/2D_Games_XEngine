#include "Pickup.h"
#include "CollisionManager.h"
#include "Enum.h"

Pickup::Pickup()
	: Entity()
	, Collidable()
	, mIsActive(false)
	, mRemoveCollider(false)
	, mImageId(0)
	, mPosition(0.0f, 0.0f)
{
}

Pickup::~Pickup()
{
}

void Pickup::Load()
{
	mImageId = X::LoadTexture("qmark.png");
	mIsActive = false;
	mRemoveCollider = false;
}

void Pickup::Update(float deltaTime)
{
	if (mRemoveCollider)
	{
		CollisionManager::Get()->RemoveCollidable(this);
		mRemoveCollider = false;
	}
}

void Pickup::Render()
{
	if (mIsActive)
	{
		X::DrawSprite(mImageId, mPosition);
	}
}

void Pickup::Unload()
{
}

int Pickup::GetType() const
{
	return mIsActive ? ET_PICKUP : 0;
}

void Pickup::OnCollision(Collidable* collidable)
{
	if(mIsActive)
	{
		mIsActive = false;
		mRemoveCollider = true;
	}
}

const X::Math::Vector2& Pickup::GetPosition() const
{
	return mPosition;
}

bool Pickup::IsActive() const
{
	return mIsActive;
}

void Pickup::SetActive(const X::Math::Vector2& position)
{
	mIsActive = true;
	mPosition = position;
	float halfWidth = X::GetSpriteWidth(mImageId) * 0.5f;
	float halfHeight = X::GetSpriteHeight(mImageId) * 0.5f;
	X::Math::Rect newRect;
	newRect.left = position.x - halfWidth;
	newRect.right = position.x + halfWidth;
	newRect.top = position.y - halfHeight;
	newRect.bottom = position.y + halfHeight;

	SetRect(newRect);
	SetCollidableFilter(ET_PICKUP);
	CollisionManager::Get()->AddCollidable(this);
}
