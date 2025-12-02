#include "DashPowerUp.h"
#include "Ship.h"

void DashPowerUp::Render()
{
	if (!IsActive())
	{
		return;
	}

	const X::Math::Vector2& p = GetPosition(); // Get position from base class

	X::DrawSprite(X::LoadTexture("dash_powerup.png"), p);
	X::DrawScreenCircle(p, GetRadius(), X::Colors::Magenta);
}

void DashPowerUp::OnCollition(Collidable* collidable)
{
	Ship* ship = dynamic_cast<Ship*>(collidable);
	if (ship)
	{
		ship->AddDash(2); // Add 2 dash charges
		Deactivate();
	}
}