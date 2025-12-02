#include "TileMap.h"

TileMap::TileMap()
{
}

TileMap::~TileMap()
{
	XASSERT(mTileMap.empty(), "[TileMap] Tile map already loaded.");
}

void TileMap::Load()
{
	
}

void TileMap::Update(float deltaTime)
{
	for (Tile* tile : mTileMap)
	{
		tile->Update(deltaTime);
	}
}

void TileMap::Render()
{
	for (Tile* tile : mTileMap)
	{
		tile->Render();
	}
}

void TileMap::Unload()
{
	for (Tile* tile : mTileMap)
	{
		tile->Unload();
		delete tile;
		tile = nullptr;
	}
	mTileMap.clear();
}

void TileMap::LoadMap(const std::vector<int>& tiles, int columns, float size)
{
	XASSERT(tiles.size() % columns == 0, "[TileMap] Invalid tile map data.");
	X::Math::Vector2 position = X::Math::Vector2::Zero();
	for (int i = 0; i < tiles.size(); ++i)
	{

		if (i > 0 && i % columns == 0)
		{
			position.x = 0.0f;
			position.y += size;
		}

		Tile* newTile = new Tile((TileType)tiles[i], position);
		newTile->Load();
		mTileMap.push_back(newTile);
		position.x += size;

	}
}