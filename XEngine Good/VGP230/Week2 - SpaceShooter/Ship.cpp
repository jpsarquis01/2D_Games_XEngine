#include "Ship.h"
#include "Bullet.h"
#include "BulletPool.h"

// Implementation
Ship::Ship()
	:Entity()
	, Collidable(30.0f)
	, mImageId(0)
	, mPosition(0.0f)
	, mRotation(0.0f)
	, scale(1.0f)
	, mBulletPool(NULL)
{

}

Ship::~Ship()
{

}

void Ship::Load()
{
	mImageId = X::LoadTexture("fighter.png");
	mMaxHealth = 100;
	mHealth = mMaxHealth;

	// Points out problem 
	XASSERT(mImageId != 0, "Ship: Failed to load image");

	// Center of the screen
	mPosition.x = X::GetScreenWidth() * 0.5f;
	mPosition.y = X::GetScreenHeight() * 0.5f;

	SetCollitionFilter(ET_ENEMY | ET_BULLET_ENEMY);
}

void Ship::Update(float deltaTime)
{
	const float speed = 200.0f;
	const float turnSpeed = X::Math::kPiByTwo;
	mFireTimer = std::max(0.0f, mFireTimer - deltaTime);

	// Dash handeling
	mDashTime = std::max(0.0f, mDashTime - deltaTime);
	float dashSpeed = speed * (mDashTime > 0.0f ? mDashSpeedMul : 1.0f);

	if (X::IsKeyPressed(X::Keys::E) && mDash > 0 && mDashTime <= 0.0f)
	{
		mDashTime = 0.5f;
		--mDash;
	}

	// Forward and Backward
	if (X::IsKeyDown(X::Keys::W))
	{
		mPosition += X::Math::Vector2::Forward(mRotation) * dashSpeed * deltaTime;
	}
	else if (X::IsKeyDown(X::Keys::S))
	{
		mPosition -= X::Math::Vector2::Forward(mRotation) * dashSpeed * deltaTime;
	}

	// Left and Right
	if (X::IsKeyDown(X::Keys::A))
	{
		mRotation -= turnSpeed * deltaTime;
	}
	else if (X::IsKeyDown(X::Keys::D))
	{
		mRotation += turnSpeed * deltaTime;
	}

	// Change Weapon
	if (X::IsKeyPressed(X::Keys::Q))
	{
		mWeapon = (mWeapon == WeaponType::SingleShot) ? WeaponType::TripleShot : WeaponType::SingleShot;
	}

	// Shoot bullet
	if (X::IsKeyPressed(X::Keys::SPACE) && mFireTimer <= 0.0f && mBulletPool)
	{
		const float life = 2.0f;
		const float muzzleDistance = 50.0f;

		if (mWeapon == WeaponType::SingleShot)
		{
			X::Math::Vector2 bulletPos = mPosition + X::Math::Vector2::Forward(mRotation) * muzzleDistance;
			Bullet* bullet = mBulletPool->GetBullet();
			bullet->SetEntityType(ET_BULLET_PLAYER);
			bullet->SetActive(bulletPos, mRotation, life);
			mFireTimer = 0.15f;
		}
		else
		{
			const float spread = X::Math::kPi / 12.0f; // 15 degrees
			
			for(int i = -1; i <= 1; ++i)
			{
				float r = mRotation + i * spread;
				X::Math::Vector2 bulletPos = mPosition + X::Math::Vector2::Forward(r) * muzzleDistance;
				Bullet* bullet = mBulletPool->GetBullet();
				if (bullet)
				{
					bullet->SetEntityType(ET_BULLET_PLAYER);
					bullet->SetActive(bulletPos, r, life);
				}
			}
			mFireTimer = 0.25f;
		}
		mFireTimer = (mWeapon == WeaponType::SingleShot) ? 0.15f : 0.25f;
	}

	// Shield timer
	mShieldTime = std::max(0.0f, mShieldTime - deltaTime);
	if(mShieldTime > 0.0f)
	{
		SetCollitionFilter(ET_NONE); // No collision when shielded
	}
	else
	{
		SetCollitionFilter(ET_ENEMY | ET_BULLET_ENEMY);
	}
}

void Ship::Render()
{
	X::DrawSprite(mImageId, mPosition, mRotation);
	X::DrawScreenCircle(mPosition, GetRadius(), X::Colors::Red);
}

void Ship::Unload()
{

}

int Ship::GetType() const
{
	return ET_SHIP;
}

const X::Math::Vector2& Ship::GetPosition() const
{
	return mPosition;
}

void Ship::OnCollition(Collidable* collidable)
{
	if (IsAlive())
	{
		int damage = 0;
		if (collidable->GetType() == ET_ENEMY)
		{
			damage = 10;

		}
		else
		{
			damage = 2;
		}
		mHealth -= damage;

		if (!IsAlive())
		{
			SetCollitionFilter(0);
		}
	}
}

void Ship::SetBulletPool(BulletPool* bulletPool)
{
	mBulletPool = bulletPool;
}

int Ship::GetHealth() const
{
	return mHealth;
}

int Ship::GetMaxHealth() const
{
	return mMaxHealth;
}

bool Ship::IsAlive() const
{
	return mHealth > 0;
}

void Ship::ApplyShield(float seconds)
{
	mShieldTime = std::max(mShieldTime, seconds);
}

void Ship::DoNovaShot()
{
	if (!mBulletPool)
	{
		return;
	}

	const int bulletsPerRing = 36;
	const float life = 2.0f;
	const float muzzleDist = 50.0f;

	for (int ring = 0; ring < 2; ++ring)
	{
		for (int i = 0; i < bulletsPerRing; ++i)
		{
			float angle = (X::Math::kTwoPi * i) / bulletsPerRing;
			X::Math::Vector2 pos = mPosition + X::Math::Vector2::Forward(angle) * muzzleDist;
			Bullet* bullet = mBulletPool->GetBullet();

			if(!bullet)
			{
				continue;
			}

			bullet->SetEntityType(ET_BULLET_PLAYER);
			bullet->SetActive(pos, angle, life);
		}
	}
}