#pragma once

enum EntityType : int
{
	ET_NONE		= 0,		// 0000
	ET_PLAYER	= 1 << 0,	// 0001
	ET_TILE		= 1 << 2,	// 0010
	ET_ENEMY	= 1 << 2,	// 0100
	ET_PICKUP	= 1 << 3	// 1000
};

enum TileType : int
{
	TT_NONE	= 0,	// 0.......0000
	TT_GROUND,
	TT_WALL,
	TT_WATER
};

enum MovementState : int
{
	MS_NONE = 0,
	MS_IDLE,
	MS_WALK,
	MS_JUMP,
	MS_FALL,
};