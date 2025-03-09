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
class TileTransform : public BaseComponent, BaseTransform {
private:
	int x, y;
	int width, height;
	float scaleX, scaleY;

public:
	TileTransform(int x_ = 0, int y_ = 0, int w = 0, int h = 0)
		: x(x_), y(y_), width(w), height(h), scaleX(1.0f), scaleY(1.0f) {}

	int GetX() const { return x; }
	int GetY() const { return y; }
	int GetScaleX() const { return scaleX; }
	int GetScaleY() const { return scaleY; }
	void SetSize(int width_, int height_) { width = width_; height = height_; }
	void SetPosition(int x_, int y_) { x = x_; y = y_; }
	void SetScale(float x_, float y_) { scaleX = x_; scaleY = y_; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	Transform GetScreenTransform() 
	{ 
		return { 
			((float)x) * width - (scaleX - 1.0f) * (float)width/2.0f,
			((float)y) * height - (scaleY - 1.0f) * (float)height/2.0f,
			((float)width)*scaleX, 
			((float)height)*scaleY }; 
	}
};