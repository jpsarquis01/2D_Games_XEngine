#include "Tile.h"

Tile::Tile(TileType tileType, const X::Math::Vector2& position)
	: mTileType(tileType)
	, mPosition(position)
	, mImageId(0)
{

}

Tile::~Tile()
{
}

void Tile::Load()
{
	switch (mTileType)
	{
	case Grass: mImageId = X::LoadTexture("grass.png");
		break;
	case GrassFlower:mImageId = X::LoadTexture("flowe.png");
		break;
	case Dirt:mImageId = X::LoadTexture("grass_2.png");
		break;
	case DirtFlower:mImageId = X::LoadTexture("flower_2.png");
		break;
	case GrassTree1:mImageId = X::LoadTexture("tree01.png");
		break;
	case GrassTree2:mImageId = X::LoadTexture("tree02.png");
		break;
	case GrassTree3:mImageId = X::LoadTexture("tree03.png");
		break;
	case GrassTree4:mImageId = X::LoadTexture("tree04.png");
		break;
	case DirtTree1:mImageId = X::LoadTexture("tree01_2.png");
		break;
	case DirtTree2:mImageId = X::LoadTexture("tree02_2.png");
		break;
	case DirtTree3:mImageId = X::LoadTexture("tree03_2.png");
		break;
	case DirtTree4:mImageId = X::LoadTexture("tree04_2.png");
		break;
	default:
		break;
	}
}

void Tile::Update(float deltaTime)
{
}

void Tile::Render()
{
	X::DrawSprite(mImageId, mPosition);
}

void Tile::Unload()
{
}
