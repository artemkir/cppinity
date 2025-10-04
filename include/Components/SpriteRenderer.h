#pragma once

#include <memory>
#include "RendererComponent.h"
#include "Texture.h"
#include "Material.h"

// Sprite Renderer Class
class SpriteRenderer : public RendererComponent
{
private:
    std::shared_ptr<ITexture> texture;
    std::string materialName;
    std::shared_ptr<Material> material;

public:
    SpriteRenderer(std::shared_ptr<ITexture> texture_, int renderOrder_ = 0, const std::string& materialName = "UI/Texture");

    // No explicit cleanup needed as shared_ptr handles texture destruction
    ~SpriteRenderer() override = default;
    void Render() override;
    void Awake() override;
};
