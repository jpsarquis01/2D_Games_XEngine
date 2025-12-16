#pragma once

enum EntityType : int
{
	ET_NONE		= 0,		// 0000
	ET_PLAYER	= 1 << 0,	// 0001
	ET_TILE		= 1 << 1,	// 0010
	ET_ENEMY	= 1 << 2,	// 0100
	ET_PICKUP	= 1 << 3,	// 1000
	ET_BULLET = 1 << 4,     // 10000
	ET_PUNCH = 1 << 5       // 100000
};

enum TileType : int
{
	TT_NONE	= 0,	// 0.......0000
	TT_GROUND,		// 1
	TT_WALL,		// 2
	TT_WATER,		// 3
	TT_DESTRUCTIBLE_3,  // 4 - Breaks after 3 hits
	TT_DESTRUCTIBLE_5   // 5 - Breaks after 5 hits
};

enum WeaponType : int
{
    WT_PUNCH = 0,  // Default weapon, always available
    WT_GUN         // Must be picked up first
};

enum PickupType : int
{
	PT_AMMO = 0,
	PT_GUN
};