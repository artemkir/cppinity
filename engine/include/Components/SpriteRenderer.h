#pragma once

#include "Std.h"

#include "RendererComponent.h"
#include "Texture.h"
#include "Material.h"

// Sprite Renderer Class
class SpriteRenderer : public RendererComponent
{
private:
    String materialName;
    String textureName;

    SharedPtr<Texture> texture;
    SharedPtr<Material> material;

public:
    explicit SpriteRenderer(const String& texture_, int renderOrder_ = 0, const String& materialName = "UI/Texture");

    // No explicit cleanup needed as shared_ptr handles texture destruction
    ~SpriteRenderer() override = default;
    void Render() override;
    void Awake() override;
};
