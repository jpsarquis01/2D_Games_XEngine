#pragma once
#include "PowerUp.h"

class Ship;

class ShieldPowerUp : public PowerUp
{
public:
	ShieldPowerUp() : PowerUp(20.0f) {}

	void Render() override;
	void OnCollition(Collidable* collidable) override;

};
