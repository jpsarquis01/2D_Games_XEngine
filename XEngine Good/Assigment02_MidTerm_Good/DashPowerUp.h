#pragma once
#include "PowerUp.h"

class Ship;

class DashPowerUp : public PowerUp
{
	public:
	DashPowerUp() : PowerUp(20.0f) {}
	void Render() override;
	void OnCollition(Collidable* collidable) override;

};