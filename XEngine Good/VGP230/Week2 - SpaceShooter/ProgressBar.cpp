#include "ProgressBar.h"

ProgressBar::ProgressBar()
	:Entity()
	, mBackgroundImageId(0)
	, mBarId(0)
	, mBarRect()
	, mPosition(0.0f)
	, mBarFullWidth(0.0f)
{
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::Load()
{
	mBackgroundImageId = X::LoadTexture("bar_empty.png");
	mBarId = X::LoadTexture("bar_red.png");
	mBarRect.right = X::GetSpriteWidth(mBarId);
	mBarRect.bottom = X::GetSpriteHeight(mBarId);
	mBarFullWidth = mBarRect.right;
	mPosition.x = (X::GetScreenWidth() - mBarFullWidth) * 0.5f;
	mPosition.y = 20.0f;
}

void ProgressBar::Update(float deltaTime)
{
}

void ProgressBar::Render()
{
	X::DrawSprite(mBackgroundImageId, mPosition, X::Pivot::Left);
	X::DrawSprite(mBarId, mBarRect, mPosition, X::Pivot::Left);
}

void ProgressBar::Unload()
{
}

void ProgressBar::SetProgress(float progress)
{
}

void ProgressBar::SetBarValue(int current, int max)
{
	mBarRect.right = X::Math::Clamp((float)current / float(max), 0.0f, 1.0f) * mBarFullWidth;
}
