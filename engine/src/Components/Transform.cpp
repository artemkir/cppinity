#include "Components/Transform.h"

GridTransform::GridTransform(float x_, float y_, float w, float h) :
    t{ { x_, y_ } , { w, h } , { 1.0f, 1.0f } }
{
}

float GridTransform::GetX() const { return t.pos.x; }
float GridTransform::GetY() const { return t.pos.y; }
float GridTransform::GetScaleX() const { return t.scale.x; }
float GridTransform::GetScaleY() const { return t.scale.y; }

void GridTransform::SetSize(float width_, float height_)
{
    t.size = { width_, height_ };
}

void GridTransform::SetPosition(float x_, float y_)
{
    t.pos = { x_,y_ };
}

void GridTransform::SetScale(float x_, float y_)
{
	t.scale = { x_,y_ };
}

float GridTransform::GetWidth() const { return t.size.x; }
float GridTransform::GetHeight() const { return t.size.y; }

BaseTransform::Transform GridTransform::GetScreenTransform() const
{
    Vector2 one{ 1.0f, 1.0f };
    Vector2 scaledSize = t.size * t.scale;
    Vector2 shift = (t.scale - one) * t.size * 0.5f;
	Vector2 screenPos = t.pos * t.size;   

	return { { screenPos - shift}, { scaledSize }, {0,0} };
}