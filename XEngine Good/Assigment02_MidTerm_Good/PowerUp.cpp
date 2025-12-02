#include "PowerUp.h"
#include <XEngine.h>

void PowerUp::Render()
{
	if(!mActive)
	{
		return;
	}
	
	X::DrawSprite(X::LoadTexture("powerup.png"), mPosition);
	X::DrawScreenCircle(mPosition, GetRadius(), X::Colors::Yellow);
}

void PowerUp::OnCollition(Collidable*)
{
	Deactivate();
}

void PowerUp::SpawnRandom()
{
	// Random position within screen bounds
	float w = (float)X::GetScreenWidth();
	float h = (float)X::GetScreenHeight();
	mPosition.x = X::RandomFloat(50.0f, w - 50.0f);
	mPosition.y = X::RandomFloat(50.0f, h - 50.0f);
	mActive = true;
	SetCollitionFilter(ET_SHIP); // Enable collision with ship
}