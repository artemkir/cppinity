#pragma once

#include <array>
#include "../Scene.h"

// Renderer Component
class RectRenderer : public RendererComponent {
private:
		
	struct Color
	{
		Uint8 r, g, b;
	} color;


public:
    RectRenderer(Uint8 r_, Uint8 g_, Uint8 b_, int renderOrder_ = 0) :
        RendererComponent(renderOrder_),
        color{ r_,g_,b_ }
    {}
    
	Color GetColor()
	{
		return color;
	}

	void Render() override {
        auto t = gameObject->GetTransform()->GetScreenTransform();
        const auto& renderer = GetRenderer();

		renderer.SetDrawColor(color.r, color.g, color.b, 255);
		renderer.DrawRect(t.x, t.y, t.width, t.height);
	}
};

// Sprite Renderer Class
class SpriteRenderer : public RendererComponent {
private:
    std::shared_ptr<ITexture> texture;
    
public:
    SpriteRenderer(std::shared_ptr<ITexture> texture_, int renderOrder_ = 0)
        : RendererComponent(renderOrder_), 
            texture(std::move(texture_))
    {
    }

    void Start() override {
        
    }

    ~SpriteRenderer() override = default;

    void Render() override {
        auto t = gameObject->GetTransform()->GetScreenTransform();
        const auto& renderer = GetRenderer();

        renderer.DrawTexture(t.x, t.y, t.width, t.height, *texture);
    }
};