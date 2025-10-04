#include "Components/SpriteRenderer.h"
#include "GameObject.h"
#include "Components/Transform.h"
#include "IRenderer.h"
#include "Scene.h"
#include "MaterialManager.h"
#include "TexturesManager.h"

extern "C" int sokol_get_screen_width();
extern "C" int sokol_get_screen_height();

SpriteRenderer::SpriteRenderer(
    const std::string& texture_,
    int renderOrder_,
    const std::string& materialName)
    : RendererComponent(renderOrder_), textureName(texture_), materialName(materialName) {}

void SpriteRenderer::Awake()
{
    material = gameObject->GetScene()->GetMaterialManager()->GetMaterial(materialName);
    texture = gameObject->GetScene()->GetTextureManager()->GetTexture(textureName);
}

void SpriteRenderer::Render()
{
    auto transform = gameObject->GetTransform()->GetScreenTransform();
    const auto& renderer = GetRenderer();

    material->SetUniform("u_color", { 1.0f, 1.0f, 1.0f, 1.0f });
    material->SetUniform("u_pixel_top_left", { transform.x, transform.y });
    material->SetUniform("u_pixel_size", { transform.width, transform.height });
    float sw = static_cast<float>(sokol_get_screen_width());
    float sh = static_cast<float>(sokol_get_screen_height());
    material->SetUniform("u_screen_size", { sw, sh });

    renderer->ApplyMaterial(material.get());
    renderer->ApplyTexture(texture);
    renderer->Draw(6);
}