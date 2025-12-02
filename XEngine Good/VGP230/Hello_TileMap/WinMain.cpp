#include <XEngine.h>
#include "TileMap.h"

// g is for global
// c is for const
// s is for static
// m is for member
// i is for int
// f is for float

TileMap gTileMap;

std::vector<int> gTiles =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void GameInit()
{
	gTileMap.Load();
	gTileMap.LoadMap(gTiles, 10, 32.0f);
}

void GameRender()
{
	gTileMap.Render();
}

bool GameUpdate(float deltaTime)
{
	gTileMap.Update(deltaTime);

	GameRender();

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

void GameCleanup()
{
	gTileMap.Unload();
}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");
	GameInit();

	X::Run(GameUpdate);

	GameCleanup();
	X::Stop();

	return 0;
}