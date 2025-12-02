#include "Enemy.h"
#include "Enum.h"
#include "TileMap.h"
#include "CollisionManager.h"

Enemy::Enemy()
	: Entity()
	, Collidable()
	, mImageId(0)
	, mPosition(0.0f, 0.0f)
	, mCenterPoint(0.0f, 0.0f)
	, mTargetPoint(0.0f, 0.0f)
	, mHealth(0)
	, mTargetPointUpdate(0.0f)
	, mRemoveCollider(false)
{
}

Enemy::~Enemy()
{
}

void Enemy::Load()
{
	mImageId = X::LoadTexture("stone.png");
	mTargetPointUpdate = 0.0f;
	mHealth = -1;
	mRemoveCollider = false;     

	float halfWidth = X::GetSpriteWidth(mImageId) * 0.5f;
	float halfHeight = X::GetSpriteHeight(mImageId) * 0.5f;
	mEnemyRect.left = -halfWidth;
	mEnemyRect.right = halfWidth;
	mEnemyRect.top = -halfHeight;
	mEnemyRect.bottom = halfHeight;
}

void Enemy::Update(float deltaTime)
{
	if (mRemoveCollider)
	{
		CollisionManager::Get()->RemoveCollidable(this);
		mRemoveCollider = false;
	}

	if (!IsActive())
	{
		return;
	}

	const float speed = 70.0f;
	const float offsetDistance = 200.0f;
	mTargetPointUpdate -= deltaTime;

	if(mTargetPointUpdate <= 0.0f || X::Math::Vector2::SqrMagnitude(mTargetPoint - mPosition) <= 100.0f)
	{
		mTargetPoint = mCenterPoint + (X::RandomUnitCircle() * offsetDistance);
		mTargetPointUpdate = X::RandomFloat(2.0f, 5.0f);
	}

	X::Math::Vector2 direction = X::Math::Normalize(mTargetPoint - mPosition);
	if(X::Math::Vector2::SqrMagnitude(direction) > 0.0f)
	{
		X::Math::Vector2 displacement = direction * speed * deltaTime;
		X::Math::Vector2 maxDisplacement = displacement;
		X::Math::Rect currentRect = mEnemyRect;
		currentRect.min += mPosition;
		currentRect.max += mPosition;
		if (TileMap::Get()->HasCollision(currentRect, maxDisplacement, displacement))
		{
			// hit wall
			mPosition += displacement;
			if (X::Math::Vector2::SqrMagnitude(displacement) <= 10.01f)
			{
				mTargetPointUpdate = 0.0f;
			}
		}
		else
		{
			mPosition += displacement;
		}
		currentRect = mEnemyRect;
		currentRect.min += mPosition;
		currentRect.max += mPosition;
		SetRect(currentRect);
	}
}

void Enemy::Render()
{
	if (IsActive())
	{
		X::DrawSprite(mImageId, mPosition);
	}
}

void Enemy::Unload()
{
}

int Enemy::GetType() const
{
	return (IsActive()) ? ET_ENEMY : ET_NONE;
}

void Enemy::OnCollision(Collidable* collidable)
{
	if (IsActive())
	{
		if (collidable->GetType() == ET_PLAYER)
		{
			mHealth -= 10;
			mRemoveCollider = true;
		}
	}
}

const X::Math::Vector2& Enemy::GetPosition() const
{
	return mPosition;
}

bool Enemy::IsActive() const
{
	return mHealth > 0;
}

void Enemy::SetActive(const X::Math::Vector2& position, int health)
{
	mPosition = position;
	mHealth = health;
	mCenterPoint = position;
	mTargetPointUpdate = 0.0f;
	mTargetPoint = position;  

	X::Math::Rect CurrentRect = mEnemyRect;
	CurrentRect.min += mPosition;
	CurrentRect.max += mPosition;
	SetRect(CurrentRect);
	SetCollidableFilter(ET_PLAYER);

	CollisionManager::Get()->AddCollidable(this);
	mRemoveCollider = false;
}