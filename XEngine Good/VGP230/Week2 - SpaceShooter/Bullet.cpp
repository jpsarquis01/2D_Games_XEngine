#include "Bullet.h"

Bullet::Bullet()
	:Entity()
	, Collidable(10.0f)
	, mImageId(0)
	, mPosition(0.0f)
	, mRotation(0.0f)
	, mlifeTime(0.0f)
	, mBulletType(ET_ALL)
{

}

Bullet::~Bullet()
{

}

void Bullet::Load()
{
	mImageId = X::LoadTexture("bullet1.png");
}

void Bullet::Update(float deltaTime) 
{
	if (IsActive())
	{
		mlifeTime -= deltaTime;
		const float speed = 500.0f;
		mPosition += X::Math::Vector2::Forward(mRotation) * speed * deltaTime;

		if (!IsActive())
		{
			SetCollitionFilter(ET_NONE);
		}
	}
}

void Bullet::Render() 
{
	if (IsActive())
	{
		X::DrawSprite(mImageId, mPosition, mRotation);
		X::DrawScreenCircle(mPosition, GetRadius(), X::Colors::Red);
	}
}

void Bullet::Unload()
{ 

}

void Bullet::SetActive(const X::Math::Vector2& pos, float rotation, float lifeTime)
{
	mPosition = pos;
	mRotation = rotation;
	mlifeTime = lifeTime;
}

void Bullet::SetEntityType(EntityType entitytype)
{
	mBulletType = entitytype;
	if (entitytype == ET_BULLET_PLAYER)
	{
		SetCollitionFilter(ET_ENEMY);
	}
	else if (entitytype == ET_BULLET_ENEMY)
	{
		SetCollitionFilter(ET_SHIP);
	}
	else
	{
		SetCollitionFilter(ET_NONE);
	}
}

bool Bullet::IsActive() const
{
	return mlifeTime > 0.0f;
}

int Bullet::GetType() const
{
	return mBulletType;
}

const X::Math::Vector2& Bullet::GetPosition() const
{
	return mPosition;
}

void Bullet::OnCollition(Collidable* collidable)
{
	mlifeTime = 0.0f;
	SetCollitionFilter(ET_NONE);
}