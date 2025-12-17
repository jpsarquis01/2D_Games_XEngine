#include "Tile.h"

Tile::Tile(TileType tileType, const X::Math::Vector2& position)
	: Entity()
	, mTileType(tileType)
	, mPosition(position)
	, mRect(0, 0, 0, 0)
	, mImageId(0)
	, mImageId5Hit(0)
	, mGroundImageId(0)
	, mMaxHits(0)
	, mHitsRemaining(0)
	, mIsDestroyed(false)
{
	// Set max hits based on tile type
	if (tileType == TT_DESTRUCTIBLE_3)
	{
		mMaxHits = 3;
		mHitsRemaining = 3;
	}
	else if (tileType == TT_DESTRUCTIBLE_5)
	{
		mMaxHits = 5;
		mHitsRemaining = 5;
	}
}

Tile::~Tile()
{
}

void Tile::Load()
{
	// Load ground texture for all destructible tiles
	if (mTileType == TT_DESTRUCTIBLE_3 || mTileType == TT_DESTRUCTIBLE_5)
	{
		mGroundImageId = X::LoadTexture("Floor.png");
	}

	switch (mTileType)
	{
	case TT_GROUND: mImageId = X::LoadTexture("Floor.png"); break;
	case TT_WALL: mImageId = X::LoadTexture("Wall.png"); break;
	case TT_WATER: mImageId = X::LoadTexture("cyan.jpg"); break;
	case TT_DESTRUCTIBLE_3: mImageId = X::LoadTexture("Chair.png"); break;
	case TT_DESTRUCTIBLE_5: 
		mImageId = X::LoadTexture("Table.png"); // 5 hit destructable tile
		mImageId5Hit = mImageId; 
		break;
	case TT_NONE:
		XASSERT(false, "Tile type is none!");
	}

	float halfWidth = X::GetSpriteWidth(mImageId) * 0.5f;
	float halfHeight = X::GetSpriteWidth(mImageId) * 0.5f;
	mRect.left = mPosition.x - halfWidth;
	mRect.right = mPosition.x + halfWidth;
	mRect.top = mPosition.y - halfHeight;
	mRect.bottom = mPosition.y + halfHeight;
}

void Tile::Update(float deltaTime)
{

}

void Tile::Render()
{
	// Always render something (ground if destroyed, original tile if not)
	if (mIsDestroyed)
	{
		// Render ground tile where destructible used to be
		X::DrawSprite(mGroundImageId, mPosition);
	}
	else
	{
		X::DrawSprite(mImageId, mPosition);
		
		// Show hits remaining for destructible tiles
		if (IsDestructible())
		{
			char hitsText[32];
			sprintf_s(hitsText, "%d", mHitsRemaining);
			
			// Different colors based on damage taken
			X::Math::Vector4 textColor = X::Colors::White;
			if (mTileType == TT_DESTRUCTIBLE_3)
			{
				if (mHitsRemaining == 2) textColor = X::Colors::Yellow;
				else if (mHitsRemaining == 1) textColor = X::Colors::Red;
			}
			else if (mTileType == TT_DESTRUCTIBLE_5)
			{
				if (mHitsRemaining <= 2) textColor = X::Colors::Red;
				else if (mHitsRemaining <= 3) textColor = X::Colors::Orange;
				else if (mHitsRemaining == 4) textColor = X::Colors::Yellow;
			}
			
			X::DrawScreenText(hitsText, mPosition.x - 5.0f, mPosition.y - 10.0f, 18.0f, textColor);
		}
	}
}

void Tile::Unload()
{

}

const X::Math::Vector2& Tile::GetPosition() const
{
	return mPosition;
}

bool Tile::IsCollidable() const
{
	if (IsDestructible() && mIsDestroyed)
	{
		return false; // Ground tiles are walkable
	}
	return mTileType == TT_WALL || IsDestructible();
}

bool Tile::HasCollision(const X::Math::Rect& objRect) const
{
	if (mIsDestroyed)
	{
		return false; // No collision with ground tiles
	}
	return X::Math::Intersect(mRect, objRect);
}

const X::Math::Rect& Tile::GetRect() const
{
	return mRect;
}

void Tile::TakeDamage(int damage)
{
	if (IsDestructible() && !mIsDestroyed)
	{
		mHitsRemaining -= damage;
		if (mHitsRemaining <= 0)
		{
			mIsDestroyed = true;
			mHitsRemaining = 0;
			ConvertToGround(); // Convert to ground when destroyed
		}
	}
}

bool Tile::IsDestructible() const
{
	return mTileType == TT_DESTRUCTIBLE_3 || mTileType == TT_DESTRUCTIBLE_5;

	// Tile Takes Damage -> Reduce Hits Remaining -> Hits = 0? 
	// -> YES : ConvertToGround() + Become Walkable
	//    -> NO : Continue showing damaged tile
}

void Tile::ConvertToGround()
{
	// Change tile type to ground
	mTileType = TT_GROUND;
	mImageId = mGroundImageId;
	mIsDestroyed = true;
}
