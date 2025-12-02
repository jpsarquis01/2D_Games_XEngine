#pragma once
#include "Entity.h"

class ProgressBar : public Entity
{
public:
	ProgressBar();
	~ProgressBar() override;
	// Entity Functions
	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;
	void SetProgress(float progress);

	void SetBarValue(int current, int max);

private:
	X::TextureId mBackgroundImageId;
	X::TextureId mBarId;
	X::Math::Rect mBarRect;
	X::Math::Vector2 mPosition;
	float mBarFullWidth;
};