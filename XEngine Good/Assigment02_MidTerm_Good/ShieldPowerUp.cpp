#include "ShieldPowerUp.h"
#include "Ship.h"

void ShieldPowerUp::Render()
{
	if (!IsActive())
	{
		return;
	}

	const X::Math::Vector2& p = GetPosition(); // Get position from base class

	X::DrawSprite(X::LoadTexture("shield_powerup.png"), p);
	X::DrawScreenCircle(p, GetRadius(), X::Colors::Cyan);
}

void ShieldPowerUp::OnCollition(Collidable* collidable)
{
	Ship* ship = dynamic_cast<Ship*>(collidable);
	if(ship)
	{
		ship->ApplyShield(5.0f); // Apply shield for 5 seconds
		Deactivate();
	}
}

