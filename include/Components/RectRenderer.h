#pragma once

#include "RendererComponent.h"
#include <memory>           
#include "Material.h"       

// Renderer Component
class RectRenderer : public RendererComponent
{
    struct Color
    {
        unsigned char r, g, b;
    };

	Color color;
    std::string materialName;
    std::shared_ptr<Material> material;

public:
    RectRenderer(
        unsigned char r_, 
        unsigned char g_, 
        unsigned char b_, 
        int renderOrder_ = 0,
        const std::string &materialName = "rect_material");

    Color GetColor();
    void Render() override;
	void Awake() override;
};