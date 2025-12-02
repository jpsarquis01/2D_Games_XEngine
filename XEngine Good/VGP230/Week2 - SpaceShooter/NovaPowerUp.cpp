#include "NovaPowerUp.h"
#include "Ship.h"

void NovaPowerUp::Render()
{
	if (!IsActive())
	{
		return;
	}

	const X::Math::Vector2& p = GetPosition(); // Get position from base class

	X::DrawSprite(X::LoadTexture("nova_powerup.png"), p);
	X::DrawScreenCircle(p, GetRadius(), X::Colors::Orange);
}

void NovaPowerUp::OnCollition(Collidable* collidable)
{
	Ship* ship = dynamic_cast<Ship*>(collidable);
	if (ship)
	{
		ship->DoNovaShot(); // Trigger Nova Shot
		Deactivate();
	}
}
