#include <XEngine.h>

// Image 1
X::TextureId imageId = 0;
X::Math::Vector2 imagePosition = { 0.0f, 0.0f };
float speedX = 200.0f;
float speedY = 200.0f;
float imageWidth = 0.0f;
float imageHeight = 0.0f;

// Image 2
X::TextureId imageId2 = 0;
X::Math::Vector2 imagePosition2 = { 0.0f, 0.0f };
float speedX2 = 400.0f;
float speedY2 = 400.0f;
float imageWidth2 = 0.0f;
float imageHeight2 = 0.0f;

void GameInit()
{
	//Image 1
	imageId = X::LoadTexture("Rebels_Logo.jpeg");
	imageWidth = X::GetSpriteWidth(imageId);
	imageHeight = X::GetSpriteHeight(imageId);
	imagePosition.x = (X::GetScreenWidth() - imageWidth) * 0.5f;
	imagePosition.y = (X::GetScreenHeight() - imageHeight) * 0.5f;


	//Image 2
	imageId2 = X::LoadTexture("Empire_Logo.jpeg");
	imageWidth2 = X::GetSpriteWidth(imageId2);
	imageHeight2 = X::GetSpriteHeight(imageId2);
	imagePosition2.x = (X::GetScreenWidth() - imageWidth2) * 0.25f;
	imagePosition2.y = (X::GetScreenHeight() - imageHeight2) * 0.25f;

	X::Zoom(0.5f);

}

void GameRender()
{
	X::DrawSprite(imageId, imagePosition);
	X::DrawSprite(imageId2, imagePosition2);
}

bool GameLoop(float deltatime)
{
	// Image1 Movement
	imagePosition.x += speedX * deltatime;
	imagePosition.y += speedY * deltatime;

	// Image2 Movement
	imagePosition2.x += speedX2 * deltatime;
	imagePosition2.y += speedY2 * deltatime;

	// Image 1 Boundary Check
	//Horizontal
	if (imagePosition.x - imageWidth <= 0.0f)
	{
		imagePosition.x = imageWidth;
		speedX = -speedX;
	}
	else if (imagePosition.x + imageWidth >= X::GetScreenWidth())
	{
		imagePosition.x = X::GetScreenWidth() - imageWidth;
		speedX = -speedX;
	}
	//Vertical
	if (imagePosition.y - imageHeight <= 0.0f)
	{
		imagePosition.y = imageHeight;
		speedY = -speedY;
	}
	else if (imagePosition.y + imageHeight >= X::GetScreenHeight())
	{
		imagePosition.y = X::GetScreenHeight() - imageHeight;
		speedY = -speedY;
	}

	// Image 2 Boundary Check
	//Horizontal
	if (imagePosition2.x - imageWidth2 <= 0.0f)
	{
		imagePosition2.x = imageWidth2;
		speedX2 = -speedX2;
	}
	else if (imagePosition2.x + imageWidth2 >= X::GetScreenWidth())
	{
		imagePosition2.x = X::GetScreenWidth() - imageWidth2;
		speedX2 = -speedX2;
	}
	//Vertical
	if (imagePosition2.y - imageHeight2 <= 0.0f)
	{
		imagePosition2.y = imageHeight2;
		speedY2 = -speedY2;
	}
	else if (imagePosition2.y + imageHeight2 >= X::GetScreenHeight())
	{
		imagePosition2.y = X::GetScreenHeight() - imageHeight2;
		speedY2 = -speedY2;
	}

	// Do Framework
	GameRender();

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

void GameCleanup()
{
	XLOG("Game Cleanup");
}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();

	return 0;
}
