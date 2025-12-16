#include "Enemy.h"
#include "Enum.h"
#include "TileMap.h"
#include "CollisionManager.h"
#include "Bullet.h"

Enemy::Enemy()
	: Entity()
	, Collidable()
	, mImageId(0)
	, mBossImageId(0)
	, mPosition(0.0f, 0.0f)
	, mCenterPoint(0.0f, 0.0f)
	, mTargetPoint(0.0f, 0.0f)
	, mHealth(0)
	, mTargetPointUpdate(0.0f)
	, mRemoveCollider(false)
	, mKnockbackVelocity(0.0f, 0.0f)
	, mKnockbackDuration(0.0f)
	, mIsBoss(false)
	, mPlayerPosition(0.0f, 0.0f)
{
}

Enemy::~Enemy()
{
}

void Enemy::Load()
{
	mImageId = X::LoadTexture("Enemy.png");
	mBossImageId = X::LoadTexture("Boss.jpg"); 
	mTargetPointUpdate = 0.0f;
	mHealth = -1;
	mRemoveCollider = false;
	mIsBoss = false;

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

	// Handle knockback with wall collision and tile breaking
	if (mKnockbackDuration > 0.0f)
	{
		mKnockbackDuration -= deltaTime;
		
		X::Math::Vector2 displacement = mKnockbackVelocity * deltaTime;
		X::Math::Vector2 maxDisplacement = displacement;
		X::Math::Rect currentRect = mEnemyRect;
		currentRect.min += mPosition;
		currentRect.max += mPosition;
		
		// Check for wall collision during knockback
		if (TileMap::Get()->HasCollision(currentRect, maxDisplacement, displacement))
		{
			// Hit a wall, try to break destructible tiles
			X::Math::Vector2 impactPosition = mPosition + X::Math::Normalize(mKnockbackVelocity) * 32.0f;
			TileMap::Get()->DamageDestructibleTilesAtPosition(impactPosition, 1);
			
			// Stop knockback
			mPosition += displacement;
			mKnockbackDuration = 0.0f;
			mKnockbackVelocity = X::Math::Vector2::Zero();
		}
		else
		{
			mPosition += displacement;
		}
		
		currentRect = mEnemyRect;
		currentRect.min += mPosition;
		currentRect.max += mPosition;
		SetRect(currentRect);
		return;
	}

	// Movement speed - boss is faster
	float speed = mIsBoss ? 100.0f : 70.0f;
	mTargetPointUpdate -= deltaTime;

	// Update target point to chase player
	if(mTargetPointUpdate <= 0.0f)
	{
		// Chase the player directly
		mTargetPoint = mPlayerPosition;
		mTargetPointUpdate = CHASE_UPDATE_RATE;
	}

	// Move towards target (player position)
	X::Math::Vector2 direction = mTargetPoint - mPosition;
	float distanceToTarget = X::Math::Magnitude(direction);
	
	// Only move if not too close to target
	if(distanceToTarget > 5.0f)
	{
		direction = X::Math::Normalize(direction);
		X::Math::Vector2 displacement = direction * speed * deltaTime;
		X::Math::Vector2 maxDisplacement = displacement;
		X::Math::Rect currentRect = mEnemyRect;
		currentRect.min += mPosition;
		currentRect.max += mPosition;
		
		if (TileMap::Get()->HasCollision(currentRect, maxDisplacement, displacement))
		{
			// Hit wall during normal movement - damage destructible tiles occasionally
			if (X::RandomFloat(0.0f, 1.0f) < 0.1f) // 10% chance per frame when hitting wall
			{
				X::Math::Vector2 impactPosition = mPosition + direction * 32.0f;
				TileMap::Get()->DamageDestructibleTilesAtPosition(impactPosition, 1);
			}
			
			mPosition += displacement;
			if (X::Math::Vector2::SqrMagnitude(displacement) <= 10.01f)
			{
				// If stuck, update target sooner
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
		X::TextureId texToUse = mIsBoss ? mBossImageId : mImageId;
		X::DrawSprite(texToUse, mPosition);
		
		// Show boss health
		if (mIsBoss)
		{
			char healthText[32];
			sprintf_s(healthText, "BOSS HP: %d", mHealth);
			X::DrawScreenText(healthText, mPosition.x - 40.0f, mPosition.y - 50.0f, 16.0f, X::Colors::Red);
		}
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
	if (!IsActive())
	{
		return;
	}

	if (collidable->GetType() == ET_PLAYER)
	{
		if (!mIsBoss)
		{
			mHealth -= 10;
			if (mHealth <= 0)
			{
				mHealth = 0;
				mRemoveCollider = true;
			}
		}
	}

	// Bullet hitting enemy
	if (collidable->GetType() == ET_BULLET)
	{
		mHealth -= 25;
		if (mHealth <= 0)
		{
			mHealth = 0;
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

void Enemy::SetActive(const X::Math::Vector2& position, int health, bool isBoss)
{
	mPosition = position;
	mHealth = health;
	mCenterPoint = position;
	mTargetPointUpdate = 0.0f;
	mTargetPoint = position;
	mIsBoss = isBoss;
	mKnockbackDuration = 0.0f;
	mKnockbackVelocity = X::Math::Vector2::Zero();

	X::Math::Rect CurrentRect = mEnemyRect;
	if (mIsBoss)
	{
		// Make boss bigger
		CurrentRect.left *= 2.0f;
		CurrentRect.right *= 2.0f;
		CurrentRect.top *= 2.0f;
		CurrentRect.bottom *= 2.0f;
	}
	CurrentRect.min += mPosition;
	CurrentRect.max += mPosition;
	SetRect(CurrentRect);
	SetCollidableFilter(ET_ENEMY);

	CollisionManager::Get()->AddCollidable(this);
	mRemoveCollider = false;
}

void Enemy::ApplyKnockback(const X::Math::Vector2& direction, float force)
{
	if (!mIsBoss) // Boss is immune to knockback
	{
		mKnockbackVelocity = direction * force;
		mKnockbackDuration = 0.3f;
	}
	
	// Deal damage
	mHealth -= 15;
	if (mHealth <= 0)
	{
		mHealth = 0;
		mRemoveCollider = true;
	}
}

void Enemy::SetPlayerTarget(const X::Math::Vector2& playerPosition)
{
	mPlayerPosition = playerPosition;
}