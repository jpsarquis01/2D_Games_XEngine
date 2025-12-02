#pragma once
#include "Entity.h"
#include "Enum.h"

namespace X
{
	namespace Math
	{
		class Vector2;
	}
}

class Collidable
{
public:
	Collidable(float radius);
	virtual ~Collidable();

	virtual int GetType() const = 0;
	virtual const X::Math::Vector2& GetPosition() const = 0;
	virtual void OnCollition(Collidable* collidable) = 0;

	virtual void SetCollitionFilter(int filater);
	virtual int GetCollitionFilter() const;
	virtual float GetRadius() const;
	virtual bool DiCollide(const Collidable* collidable) const;

private:
	float mRadius;
	int mCollitionFilter;
};