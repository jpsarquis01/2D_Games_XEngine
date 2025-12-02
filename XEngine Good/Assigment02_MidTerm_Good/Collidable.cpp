#include "Collidable.h"
#include <XEngine.h>
#include <XMath.h>

Collidable::Collidable(float radius)
	: mRadius(radius)
	, mCollitionFilter(-1)
{

}

Collidable::~Collidable()
{

}

void Collidable::SetCollitionFilter(int filter)
{
	mCollitionFilter = filter;
}

int Collidable::GetCollitionFilter() const
{
	return mCollitionFilter;
}

float Collidable::GetRadius() const
{
	return mRadius;
}

bool Collidable::DiCollide(const Collidable* collidable) const
{
	if (collidable == nullptr)
	{
		return false;
	}

	int collidableFilter = collidable->GetCollitionFilter();
	int collisionFilter = GetCollitionFilter();

	if (collidableFilter == 0 || collisionFilter == 0)
	{
		return false;
	}

	if (collidableFilter < 0 || collisionFilter < 0 || (collidableFilter & GetType()) > 0)
	{
		float disSq = X::Math::Vector2::SqrMagnitude(collidable->GetPosition() - GetPosition());
		float radiusSq = X::Math::Sqr(collidable->GetRadius() + GetRadius());
		return disSq <= radiusSq;
	}

	return false;
}
