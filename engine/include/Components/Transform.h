#pragma once

#include "Std.h"

#include "BaseComponent.h"

class Canvas;

struct Vector2
{
	float x = 0.0f;
	float y = 0.0f;

	Vector2(float x_, float y_) : x(x_), y(y_) {}
	
	friend Vector2 operator+(const Vector2& v1, const Vector2& v2) { return Vector2{ v1.x + v2.x, v1.y + v2.y }; }
	friend Vector2 operator-(const Vector2& v1, const Vector2& v2) { return Vector2{ v1.x - v2.x, v1.y - v2.y }; }
	friend Vector2 operator*(const Vector2& v1, const Vector2& v2) { return Vector2{ v1.x * v2.x, v1.y * v2.y }; }
	friend Vector2 operator/(const Vector2& v1, const Vector2& v2)
	{
		assert(v2.x != 0.0f && v2.y != 0.0f && "Division by zero");
		return Vector2{ v1.x / v2.x, v1.y / v2.y };
	}

	friend Vector2 operator*(const Vector2& v, float scalar) { return Vector2{ v.x * scalar, v.y * scalar }; }
	friend Vector2 operator/(const Vector2& v, float scalar) 
	{ 
		assert(scalar != 0.0f && "Division by zero");
		return Vector2{ v.x / scalar, v.y / scalar }; 
	}

	List<float> to_vector() const { return { x, y }; }
};

class BaseTransform : public BaseComponent
{
public:
	struct Transform
	{
		Vector2 pos;
		Vector2 size;
		Vector2 scale;
	};

	virtual ~BaseTransform() = default;
	virtual Transform GetScreenTransform() const = 0;
	virtual Transform GetFinalScreenTransform() const = 0; // With hierarchy and canvas
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

class ScreenTransform : public BaseTransform
{
	Transform t{ {0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f} };
	ScreenTransform* parent = nullptr;
	Canvas* canvas = nullptr;

	Canvas* GetAncestorCanvas() const;
	Transform GetRelativeTransformToAncestor(const ScreenTransform* ancestor) const;

public:
	explicit ScreenTransform(float x_, float y_, float w, float h) :
		t{ { x_, y_ } , { w, h } , { 1.0f, 1.0f } }
	{}

	const Vector2& GetPos() const { return t.pos; }
	const Vector2& GetSize() const { return t.size; }
	const Vector2& GetScale() const { return t.scale; }
	float GetX() const override { return GetPos().x; }
	float GetY() const override { return GetPos().y; }
	float GetScaleX() const override { return GetScale().x; }
	float GetScaleY() const override { return GetScale().y; }
	void SetSize(float width_, float height_) override { t.size = { width_, height_ }; }
	void SetPosition(float x_, float y_) override {	t.pos = { x_,y_ }; }
	void SetScale(float x_, float y_) override {	t.scale = { x_,y_ }; }
	float GetWidth() const override { return GetSize().x; }
	float GetHeight() const override { return GetSize().y; }
	
	Transform GetScreenTransform() const override { return t; }
	Transform GetFinalScreenTransform() const override;

	void Awake() override;
};

// Grid transform component for snake
class GridTransform : public ScreenTransform
{
public:
	using ScreenTransform::ScreenTransform;

	Transform GetScreenTransform() const override
	{
		Vector2 one{ 1.0f, 1.0f };
		Vector2 scaledSize = GetSize() * GetScale();
		Vector2 shift = (GetScale() - one) * GetSize() * 0.5f;
		Vector2 screenPos = GetPos() * GetSize();

		return { { screenPos - shift}, { scaledSize }, {0,0} };
	}

	Transform GetFinalScreenTransform() const override;
};