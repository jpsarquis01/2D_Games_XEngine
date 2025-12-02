#include "Bullet.h"
#include "BulletPool.h"
#include "Ship.h"
#include "Enemy.h"
#include "AnimSpriteSheet.h"

Enemy::Enemy()
	:Entity()
	, Collidable(20.0f)
	, mBulletPool(nullptr)
	, mShip(nullptr)
	, mImageId(0)
	, mPosition(0.0f)
	, mRotation(0.0f)
	, mHealth(100)
	, mExplosionAnim(nullptr)
	, mCenterPoint(0.0f, 0.0f)
	, mTargetPosition(0.0f, 0.0f)
	, mFireRate(1.0f)
	, mTargetPositionUpdate(0.0f)
{

}
Enemy::~Enemy()
{

}

// Entity overrides
void Enemy::Load()
{
	mImageId = X::LoadTexture("carrier_01.png");
	XASSERT(mImageId > 0, "Enemy: Failed to load image");

	mPosition = X::Math::Vector2::Zero();
	mRotation = 0.0f;

	SetCollitionFilter(ET_BULLET_PLAYER | ET_SHIP);

	mExplosionAnim = new AnimSpriteSheet();
	mExplosionAnim->Load();

	mTargetPosition = mPosition;
}
void Enemy::Update(float deltaTime)
{
	if (IsAlive())
	{
		const float speed = 70.0f;
		const float rotateSpeed = X::Math::kPiByTwo;
		const float offsetDistance = 200.0f;

		mTargetPositionUpdate -= deltaTime;
		if (mTargetPositionUpdate <= 0.0f || X::Math::MagnitudeSqr(mTargetPosition - mPosition) <= 100.0f)
		{
			mTargetPosition = mCenterPoint + (X::RandomUnitCircle() * offsetDistance);
			mTargetPositionUpdate = X::RandomFloat(3.0f, 5.0f);
		}

		X::Math::Vector2 moveDirection = X::Math::Normalize(mTargetPosition - mPosition);
		mPosition += moveDirection * speed * deltaTime;

		if (mShip != nullptr && mShip->IsAlive())
		{
			moveDirection = X::Math::Normalize(mShip->GetPosition() - mPosition);
			float targetRotation = atan2(moveDirection.x, -moveDirection.y);
			mRotation = targetRotation;

			mFireRate -= deltaTime;
			if (mFireRate <= 0.0f)
			{
				Bullet* bullet = mBulletPool->GetBullet();
				if (bullet != nullptr)
				{
					X::Math::Vector2 bulletPos = mPosition + X::Math::Vector2::Forward(mRotation) * 50.0f;
					bullet->SetEntityType(ET_BULLET_ENEMY);
					bullet->SetActive(bulletPos, mRotation, 3.0f);
					mFireRate = X::RandomFloat(1.0f, 3.0f);
				}
			}
		}

	}
	mExplosionAnim->Update(deltaTime);
}
void Enemy::Render()
{
	if (IsAlive())
	{
		X::DrawSprite(mImageId, mPosition, mRotation);
		X::DrawScreenCircle(mPosition, GetRadius(), X::Colors::Red);
	}
	mExplosionAnim->Render();
}
void Enemy::Unload()
{
	mExplosionAnim->Unload();
	delete mExplosionAnim;
	mExplosionAnim = nullptr;
}

// Collidable overrides
int Enemy::GetType() const
{
	return ET_ENEMY;
}

const X::Math::Vector2& Enemy::GetPosition() const
{
	return mPosition;
}

void Enemy::OnCollition(Collidable* collidable)
{
	if (IsAlive())
	{
		int damage = 0;
		if (collidable->GetType() == ET_SHIP)
		{
			damage = mHealth;
		}
		else
		{
			damage = 50;
		}
		mHealth -= damage;

		if (!IsAlive())
		{
			mExplosionAnim->SetActive(mPosition);
			SetCollitionFilter(0);

		}
	}
}

//Behavior Functions
void Enemy::SetBulletPool(BulletPool* bulletPool)
{
	mBulletPool = bulletPool;
}
void Enemy::SetShip(Ship* ship)
{
	mShip = ship;
}
void Enemy::SetPosition(const X::Math::Vector2& position)
{
	mPosition = position;
	mCenterPoint = position;
	mTargetPosition = position;
	mTargetPositionUpdate = 0.0f;
}
void Enemy::SetRotation(float rotation)
{
	mRotation = rotation;
}

bool Enemy::IsAlive() const
{
	return mHealth > 0;
}