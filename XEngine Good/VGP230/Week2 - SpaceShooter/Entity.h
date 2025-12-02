#pragma once
#include <XEngine.h>

class Entity
{
public:
	Entity() {}
	//Inportant for not having memory leaks
	virtual ~Entity() {}

	// abstract
	// Havingit virtual means youcant do "Entity myEntity;" you have to do "class myEntity : public Entity"
	virtual void Load() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void Unload() = 0;

};
