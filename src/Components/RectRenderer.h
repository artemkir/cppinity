#pragma once

#include <array>

// Renderer Component
class RectRenderer : public IRendererComponent {
private:
	int renderOrder;
	
	struct Color
	{
		Uint8 r, g, b;
	} color;


public:
	RectRenderer(Uint8 r_, Uint8 g_, Uint8 b_, int renderOrder_ = 0) : renderOrder(renderOrder_) {
		color.r = r_;
		color.g = g_;
		color.b = b_;
	}

	int Order()
	{
		return renderOrder;
	}

	Color GetColor()
	{
		return color;
	}

	void Render(IRenderer& renderer) override {

		if (!gameObject || !gameObject->GetTransform()) return;

		renderer.SetDrawColor(color.r, color.g, color.b, 255);

		Transform* t = gameObject->GetTransform();

		renderer.DrawRect(
			t->GetX() * TILE_SIZE,
			t->GetY() * TILE_SIZE,
			t->GetWidth(),
			t->GetHeight());
	}
};