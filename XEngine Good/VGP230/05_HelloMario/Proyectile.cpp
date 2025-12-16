#include "Proyectile.h"
#include "TileMap.h"

Projectile::Projectile()
	: Entity()
	, Collidable()
	, mImageId(0)
	, mPosition(0.0f, 0.0f)
	, mLifetime(0.0f)
	, mBulletType(ET_NONE)
	, mFlip(X::Flip::None)
	, mProjectileRect()

{
}

Projectile::~Projectile()
{
}

// Entity override

void Projectile::Load()
{
	mImageId = X::LoadTexture("carrot.png");
	mLifeTime = 0.0f;
	mBulletType = ET_NONE;
	mFlip = X::Flip::None;
}

void Projectile::Update(float deltaTime)
{
	if (IsActive())
	{
		const float speed = 600.0f;
		X::Math::Vector2 displacement = mDirection * speed * deltaTime;
		X::Math::Vector2 maxDisplacement = displacement;
		if (TileMap::Get()->HasCollision(GetRect(), maxDisplacement, displacement))
		{
			mLifeTime = 0.0f;
		}

		mPosition += displacement;
		X::Math::Rect currentRect = mProjectileRect;
		currentRect.min += mPosition;
		currentRect.max += mPosition;
		SetRect(currentRect);

		mLifeTime -= deltaTime;
		if (mLifeTime <= 0.0f)
		{
			mLifeTime = 0.0f;
		}
	}
}

void Projectile::Render()
{
}

void Projectile::Unload()
{

	d
}

// Collidable override

int Projectile::GetType() const
{
	
}

void Projectile::OnCollision(Collidable* collidable)
{
}

const X::Math::Vector2& Projectile::GetPosition() const
{
	
}