#pragma once

#include <vector>
#include "BaseComponent.h"

struct Vector2
{
	float x = 0.0f;
	float y = 0.0f;

	Vector2(std::initializer_list<float> elems)
	{
		auto it = elems.begin();
		x = (it != elems.end()) ? *it++ : 0.0f;
		y = (it != elems.end()) ? *it : 0.0f;
	}
	
	friend Vector2 operator+(const Vector2& v1, const Vector2& v2) { return Vector2{ v1.x + v2.x, v1.y + v2.y }; }
	friend Vector2 operator-(const Vector2& v1, const Vector2& v2) { return Vector2{ v1.x - v2.x, v1.y - v2.y }; }
	friend Vector2 operator*(const Vector2& v1, const Vector2& v2) { return Vector2{ v1.x * v2.x, v1.y * v2.y }; }

	friend Vector2 operator*(const Vector2& v, float scalar) { return Vector2{ v.x * scalar, v.y * scalar }; }
	friend Vector2 operator/(const Vector2& v, float scalar) { return Vector2{ v.x / scalar, v.y / scalar }; }

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