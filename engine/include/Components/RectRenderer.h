#pragma once

#include "Std.h"

#include "RendererComponent.h"
#include "Material.h"       

// Renderer Component
class RectRenderer : public RendererComponent
{
    struct Color
    {
        unsigned char r, g, b;
    };

	Color color;
    String materialName;
    SharedPtr<Material> material;

public:
    RectRenderer(
        unsigned char r_, 
        unsigned char g_, 
        unsigned char b_, 
        int renderOrder_ = 0,
        const String &materialName = "UI/Color");

    Color GetColor();
    void Render() override;
	void Awake() override;
};