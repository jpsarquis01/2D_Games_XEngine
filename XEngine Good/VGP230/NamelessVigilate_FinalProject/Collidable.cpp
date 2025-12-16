#include <XEngine.h>

#include "Collidable.h"

Collidable::Collidable()
	: mCollidableFilter(-1)
{

}
Collidable::~Collidable()
{

}

void Collidable::SetCollidableFilter(int filter)
{
	mCollidableFilter = filter;
}

int Collidable::GetCollidableFilter() const
{
	return mCollidableFilter;
}

void Collidable::SetRect(const X::Math::Rect& rect)
{
	mEnemyRect = rect;
}

const X::Math::Rect& Collidable::GetRect() const
{
	return mEnemyRect;
}

bool Collidable::DidCollide(const Collidable* collidable) const
{
	if(collidable == nullptr)
	{
		return false;
	}

	int collidableFilter = collidable->GetCollidableFilter();
	int collisionFilter = GetCollidableFilter();
	if (collidable == 0 || collisionFilter == 0)
	{
		return false;
	}

	if (collidableFilter < 0 || collisionFilter < 0 || (collidableFilter & collisionFilter) > 0)
	{
		return X::Math::Intersect(mEnemyRect, collidable->GetRect());
	}

	return false;
}