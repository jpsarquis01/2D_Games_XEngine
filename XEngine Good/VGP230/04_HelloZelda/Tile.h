#pragma once

#include "Entity.h"
#include "Enum.h"

class Tile : public Entity
{
public:
	Tile(TileType tileType, const X::Math::Vector2& position);
	virtual ~Tile();

	// override functions
	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	const X::Math::Vector2& GetPosition() const;
	bool IsCollidable() const;
	bool HasCollision(const X::Math::Rect& objRect) const;
	const X::Math::Rect& GetRect() const;
	
	// Destructible tile functions
	bool IsDestructible() const;
	void TakeDamage(int damage = 1);
	bool IsDestroyed() const { return mIsDestroyed; }
	int GetMaxHits() const { return mMaxHits; }
	int GetHitsRemaining() const { return mHitsRemaining; }
	
	// Convert to ground tile
	void ConvertToGround();

private:
	TileType mTileType;
	X::Math::Vector2 mPosition;
	X::Math::Rect mRect;
	X::TextureId mImageId;
	X::TextureId mImageId5Hit; // Separate texture for 5-hit tiles
	X::TextureId mGroundImageId; // Ground tile texture
	
	// Destructible tile
	int mMaxHits;
	int mHitsRemaining;
	bool mIsDestroyed;
};