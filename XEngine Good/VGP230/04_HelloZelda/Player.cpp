#include "Player.h"
#include "TileMap.h"
#include "CollisionManager.h"


Player::Player()
	: Entity()
	, Collidable()
	, mImageId(0)
	, mPosition(0.0f, 0.0f)
	, mHealth(100)
	, mRemoveCollider(false)
	, mAmmo(0)
	, mCurrentWeapon(WT_PUNCH)
	, mHasGun(false)
	, mPunchActive(false)
	, mPunchTimer(0.0f)
	, mPunchCooldown(0.0f)
	, mPunchDirection(0.0f)
{
}

Player::~Player()
{
}

void Player::Load()
{
	mImageId = X::LoadTexture("Player.png");

	// Spawn player wherever in the screen
	mPosition.x = X::GetScreenWidth() * 0.6f;
	mPosition.y = X::GetScreenHeight() * 0.6f;

	float halfWidth = X::GetSpriteWidth(mImageId) * 0.5f;
	float halfHeight = X::GetSpriteHeight(mImageId) * 0.5f;
	mPlayerRect.left = -halfWidth;
	mPlayerRect.right = halfWidth;
	mPlayerRect.top = -halfHeight;
	mPlayerRect.bottom = halfHeight;

	// Collider Stuff
	SetRect(mPlayerRect);
	SetCollidableFilter(ET_PLAYER | ET_PICKUP | ET_ENEMY);
	mRemoveCollider = false;
	CollisionManager::Get()->AddCollidable(this);
}

void Player::Update(float deltaTime)
{
	if (mHealth <= 0)
	{
		if (mRemoveCollider)
		{
			CollisionManager::Get()->RemoveCollidable(this);
			mRemoveCollider = false;
		}
		return;
	}

	// Update punch cooldown
	if (mPunchCooldown > 0.0f)
	{
		mPunchCooldown -= deltaTime;
	}

	// Update punch timer
	if (mPunchActive)
	{
		mPunchTimer -= deltaTime;
		if (mPunchTimer <= 0.0f)
		{
			mPunchActive = false;
		}
	}

	// Weapon switching (1 for punch, 2 for gun if available)
	if (X::IsKeyPressed(X::Keys::ONE))
	{
		mCurrentWeapon = WT_PUNCH;
	}
	else if (X::IsKeyPressed(X::Keys::TWO) && mHasGun)
	{
		mCurrentWeapon = WT_GUN;
	}

	const float speed = 200.0f;
	X::Math::Vector2 direction = X::Math::Vector2::Zero();
	X::Math::Vector2 displacement = X::Math::Vector2::Zero();
	if (X::IsKeyDown(X::Keys::W))
	{
		direction.y = -1.0f;
	}
	else if (X::IsKeyDown(X::Keys::S))
	{
		direction.y = 1.0f;
	}
	if (X::IsKeyDown(X::Keys::A))
	{
		direction.x = -1.0f;
	}
	else if (X::IsKeyDown(X::Keys::D))
	{
		direction.x = 1.0f;
	}
	if (X::Math::MagnitudeSqr(direction) > 0.0f)
	{
		direction = X::Math::Normalize(direction);
		displacement = direction * speed * deltaTime;
		X::Math::Vector2 maxDisplacement = displacement;
		X::Math::Rect currentRect = mPlayerRect;
		currentRect.min += mPosition;
		currentRect.max += mPosition;
		if (TileMap::Get()->HasCollision(currentRect, maxDisplacement, displacement))
		{
			// play hit wall sound
			// play particle effect
			XLOG("Hit Wall!");
			mPosition += displacement;
		}
		else
		{
			mPosition += displacement;
		}

		currentRect = mPlayerRect;
		currentRect.min += mPosition;
		currentRect.max += mPosition;
		SetRect(currentRect);
	}
}

void Player::Render()
{
	if (mHealth > 0)
	{
		X::DrawSprite(mImageId, mPosition);
		
		// Draw punch indicator when punching
		if (mPunchActive)
		{
			X::Math::Vector2 punchPos = mPosition;
			punchPos.x += cosf(mPunchDirection) * PUNCH_RANGE;
			punchPos.y += sinf(mPunchDirection) * PUNCH_RANGE;
			X::DrawScreenCircle(punchPos, 15.0f, X::Colors::Yellow);
		}
	}
}

void Player::Unload()
{
}

int Player::GetType() const
{
	return ET_PLAYER;
}

void Player::OnCollision(Collidable* collidable)
{
	// DONT REMOVE COLLIDABLES DURING THIS FUNCTION
	if (mHealth <= 0)
	{
		return;
	}

	if (collidable->GetType() == ET_ENEMY)
	{
		mHealth -= 10;
	}
	else if (collidable->GetType() == ET_PICKUP)
	{
		// Handled by Pickup's OnCollision
	}

	mHealth = X::Math::Clamp(mHealth, 0, 100);

	if (mHealth <= 0)
	{
		mRemoveCollider = true;
	}
}

const X::Math::Vector2& Player::GetPosition() const
{
	return mPosition;
}

void Player::PerformPunch()
{
	if (mPunchCooldown > 0.0f || mPunchActive)
	{
		return;
	}

	// Get punch direction from mouse
	int mouseX = X::GetMouseScreenX();
	int mouseY = X::GetMouseScreenY();
	X::Math::Vector2 mousePos(static_cast<float>(mouseX), static_cast<float>(mouseY));
	X::Math::Vector2 dir = mousePos - mPosition;
	
	if (X::Math::MagnitudeSqr(dir) == 0.0f)
	{
		return;
	}

	dir = X::Math::Normalize(dir);
	mPunchDirection = atan2f(dir.y, dir.x);

	// Create punch hitbox
	X::Math::Vector2 punchCenter = mPosition;
	punchCenter.x += cosf(mPunchDirection) * PUNCH_RANGE;
	punchCenter.y += sinf(mPunchDirection) * PUNCH_RANGE;

	float punchSize = 30.0f;
	mPunchRect.left = punchCenter.x - punchSize;
	mPunchRect.right = punchCenter.x + punchSize;
	mPunchRect.top = punchCenter.y - punchSize;
	mPunchRect.bottom = punchCenter.y + punchSize;

	mPunchActive = true;
	mPunchTimer = PUNCH_DURATION;
	mPunchCooldown = PUNCH_COOLDOWN;

	// Player Clicks -> Calculate Direction -> Create Hitbox -> Check Enemy Collision
	// -> Apply Knockback Force -> Enemy Moves -> Hits Wall -> Damages Destructible Tiles
}
