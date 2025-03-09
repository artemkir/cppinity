#pragma once

#pragma once

#include <array>
#include "../Scene.h"

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

    //To proper texture cleanup
    ~SpriteRenderer() override = default;

    void Render() override {
        auto t = gameObject->GetTransform()->GetScreenTransform();
        const auto& renderer = GetRenderer();

        renderer.DrawTexture(t.x, t.y, t.width, t.height, *texture);
    }
};
