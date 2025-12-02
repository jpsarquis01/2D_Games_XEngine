#pragma once
#include <XEngine.h>
#include "Enums.h"

class Tile
{
public:
	Tile(TileType tileType, const X::Math::Vector2& position);
	~Tile();

	void Load();
	void Update(float deltaTime);
	void Render();
	void Unload();

private:
	TileType mTileType;
	X::Math::Vector2 mPosition;
	X::TextureId mImageId;
};