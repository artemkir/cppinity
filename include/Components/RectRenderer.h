#pragma once

#include "RendererComponent.h"

// Renderer Component
class RectRenderer : public RendererComponent
{
	struct Color
	{
		unsigned char r, g, b;
	} color;

public:
	RectRenderer(unsigned char r_, unsigned char g_, unsigned char b_, int renderOrder_ = 0);
	Color GetColor();
	void Render() override;
};