#pragma once

// Transform Controller Component
class Transform : public BaseComponent {
private:
	int x, y;
	int width, height;

public:
	Transform(int x_, int y_, int w, int h)
		: x(x_), y(y_), width(w), height(h) {}

	int GetX() const { return x; }
	int GetY() const { return y; }
	void SetPosition(int x_, int y_) { x = x_; y = y_; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
};