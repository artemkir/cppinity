#pragma once

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

// Transform Controller Component
class TileTransform : public BaseComponent, public BaseTransform {
private:
	float x;
	float y;
	float width;
	float height;
	float scaleX = 1.0f;
	float scaleY = 1.0f;

public:
	TileTransform(float x_ = 0.0f, float y_ = 0.0f, float w = 0.0f, float h = 0.0f)
		: x(x_), y(y_), width(w), height(h) {}

	float GetX() const { return x; }
	float GetY() const { return y; }
	float GetScaleX() const { return scaleX; }
	float GetScaleY() const { return scaleY; }
	void SetSize(float width_, float height_) { width = width_; height = height_; }
	void SetPosition(float x_, float y_) { x = x_; y = y_; }
	void SetScale(float x_, float y_) { scaleX = x_; scaleY = y_; }
	float GetWidth() const { return width; }
	float GetHeight() const { return height; }

	Transform GetScreenTransform() override 
	{ 
		return { 
			x * width - (scaleX - 1.0f) * width/2.0f,
			y * height - (scaleY - 1.0f) * height/2.0f,
			width*scaleX, 
			height*scaleY }; 
	}
};