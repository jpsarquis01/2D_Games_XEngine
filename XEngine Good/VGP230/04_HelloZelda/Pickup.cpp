#include "Pickup.h"
#include "CollisionManager.h"
#include "Enum.h"
#include "Player.h"

Pickup::Pickup()
	: Entity()
	, Collidable()
	, mIsActive(false)
	, mRemoveCollider(false)
	, mImageId(0)
	, mGunImageId(0)
	, mPosition(0.0f, 0.0f)
	, mPickupType(PT_AMMO)
{
}

Pickup::~Pickup()
{
}

void Pickup::Load()
{
	mImageId = X::LoadTexture("Ammo.png");
	mGunImageId = X::LoadTexture("Gun.png");
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
		if (mPickupType == PT_GUN)
		{
			X::DrawSprite(mGunImageId, mPosition);
		}
		else
		{
			X::DrawSprite(mImageId, mPosition);
		}
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
	if(mIsActive && collidable->GetType() == ET_PLAYER)
	{
		Player* player = static_cast<Player*>(collidable);
		
		if (mPickupType == PT_GUN)
		{
			player->GiveGun();
			player->AddAmmo(5);
		}
		else if (mPickupType == PT_AMMO)
		{
			player->AddAmmo(5);
		}
		
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

void Pickup::SetActive(const X::Math::Vector2& position, PickupType type)
{
	mIsActive = true;
	mPosition = position;
	mPickupType = type;
	
	X::TextureId textureToUse = (type == PT_GUN) ? mGunImageId : mImageId;
	float halfWidth = X::GetSpriteWidth(textureToUse) * 0.5f;
	float halfHeight = X::GetSpriteHeight(textureToUse) * 0.5f;
	X::Math::Rect newRect;
	newRect.left = position.x - halfWidth;
	newRect.right = position.x + halfWidth;
	newRect.top = position.y - halfHeight;
	newRect.bottom = position.y + halfHeight;

	SetRect(newRect);
	SetCollidableFilter(ET_PICKUP);
	CollisionManager::Get()->AddCollidable(this);
}
