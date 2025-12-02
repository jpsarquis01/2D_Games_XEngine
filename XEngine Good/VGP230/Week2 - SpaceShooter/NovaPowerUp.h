#pragma once
#include "PowerUp.h"

class Ship;

class NovaPowerUp : public PowerUp
{
public:
	NovaPowerUp() : PowerUp(20.0f) {}
	void Render() override;
	void OnCollition(Collidable* collidable) override;

};