#pragma once

#include <memory>
#include "RendererComponent.h"
#include "Texture.h"
#include "Material.h"

// Sprite Renderer Class
class SpriteRenderer : public RendererComponent
{
private:
    std::string materialName;
    std::string textureName;

    std::shared_ptr<Texture> texture;
    std::shared_ptr<Material> material;

public:
    explicit SpriteRenderer(const std::string& texture_, int renderOrder_ = 0, const std::string& materialName = "UI/Texture");

    // No explicit cleanup needed as shared_ptr handles texture destruction
    ~SpriteRenderer() override = default;
    void Render() override;
    void Awake() override;
};
