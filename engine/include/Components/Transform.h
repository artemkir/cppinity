#pragma once

#include <vector>
#include "BaseComponent.h"

struct Vector2
{
	float x;
	float y;

	Vector2(float x_ = 0.0f, float y_ = 0.0f) : x(x_), y(y_) {}

	Vector2 operator+(const Vector2& other) const { return { x + other.x, y + other.y }; }
	Vector2 operator-(const Vector2& other) const { return { x - other.x, y - other.y }; }
	Vector2 operator*(float scalar) const { return { x * scalar, y * scalar }; }
	Vector2 operator*(const Vector2& other) const { return { x * other.x, y * other.y }; } // Element-wise multiplication
	Vector2 operator/(float scalar) const { return { x / scalar, y / scalar }; }

	std::vector<float> to_vector() const { return { x, y }; }
};

class BaseTransform
{
public:
	struct Transform
	{
		Vector2 pos;
		Vector2 size;
		Vector2 scale;
	};

	virtual Transform GetScreenTransform() const = 0;
	virtual float GetX() const = 0;
	virtual float GetY() const = 0;
	virtual float GetScaleX() const = 0;
	virtual float GetScaleY() const = 0;
	virtual float GetWidth() const = 0;
	virtual float GetHeight() const = 0;

	virtual void SetSize(float width_, float height_) = 0;
	virtual void SetPosition(float x_, float y_) = 0;
	virtual void SetScale(float x_, float y_) = 0;
};

// Grid transform component for snake
class GridTransform : public BaseComponent, public BaseTransform
{
	Transform t;

public:
	explicit GridTransform(float x_ = 0.0f, float y_ = 0.0f, float w = 0.0f, float h = 0.0f);

	float GetX() const override;
	float GetY() const override;
	float GetScaleX() const override;
	float GetScaleY() const override;

	void SetSize(float width_, float height_) override;
	void SetPosition(float x_, float y_) override;
	void SetScale(float x_, float y_) override;
	 
	float GetWidth() const override;
	float GetHeight() const override;

	Transform GetScreenTransform() const override;
};