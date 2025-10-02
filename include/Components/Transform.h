#pragma once

#include "BaseComponent.h"

// TODO: Move to proper vector transformations
class BaseTransform
{
public:
	struct Transform
	{
		float x;
		float y;
		float width;
		float height;
	};

	virtual Transform GetScreenTransform() = 0;
};

// TODO: Move to proper vector transformations
class TileTransform : public BaseComponent, public BaseTransform
{
	float x;
	float y;
	float width;
	float height;
	float scaleX = 1.0f;
	float scaleY = 1.0f;

public:
	TileTransform(float x_ = 0.0f, float y_ = 0.0f, float w = 0.0f, float h = 0.0f);

	float GetX() const;
	float GetY() const;
	float GetScaleX() const;
	float GetScaleY() const;

	void SetSize(float width_, float height_);
	void SetPosition(float x_, float y_);
	void SetScale(float x_, float y_);

	float GetWidth() const;
	float GetHeight() const;

	Transform GetScreenTransform() override;
};