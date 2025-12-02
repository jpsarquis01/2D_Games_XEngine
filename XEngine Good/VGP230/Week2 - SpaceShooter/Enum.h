#pragma once

// Can be dynamiclly converted to bitmask
enum EntityType :int
{
	ET_ALL = -1,               // 1111 1111 1111
	ET_NONE = 0,               // 0000 0000 0000
	ET_SHIP = 1 << 0,          // 0000 0000 0001
	ET_BULLET_PLAYER = 1 << 1, // 0000 0000 0010
	ET_BULLET_ENEMY = 1 << 2,  // 0000 0000 0100
	ET_ENEMY = 1 << 3,         // 0000 0000 1000
};

enum class State
{
	Start,
	RunGame,
	End
};

//enum -> ET_NONE
//enum class -> State::Start

//int value = ET_ALL; ok
//int value = State::Start; error
//int value = (int)state; ok
