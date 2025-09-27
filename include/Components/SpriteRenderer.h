#pragma once

#include <memory>
#include "RendererComponent.h"
#include "Texture.h"

// Sprite Renderer Class
class SpriteRenderer : public RendererComponent {
private:
    std::shared_ptr<ITexture> texture;

public:
    SpriteRenderer(std::shared_ptr<ITexture> texture_, int renderOrder_ = 0);
    
    // No explicit cleanup needed as shared_ptr handles texture destruction
    ~SpriteRenderer() override = default;
    void Render() override;
};
