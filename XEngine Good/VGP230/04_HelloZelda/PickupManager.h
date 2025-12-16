#pragma once
#include "Pickup.h"

class PickupManager : public Entity
{
public:
	~PickupManager();
	static PickupManager* Get();

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	void SpawnPickup(int amount, PickupType type = PT_AMMO);

private:
	PickupManager();
	static PickupManager* mInstance;

	std::vector<Pickup*> mPickups;
	int mNextAvailablePickup;
	
	// Random spawn system
	float mSpawnTimer;
	float mNextSpawnTime;
	const float MIN_SPAWN_TIME = 5.0f;  // Minimum seconds between spawns
	const float MAX_SPAWN_TIME = 15.0f; // Maximum seconds between spawns
};