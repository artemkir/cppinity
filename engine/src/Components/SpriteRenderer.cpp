#include "Components/SpriteRenderer.h"
#include "GameObject.h"
#include "Components/Transform.h"
#include "IRenderer.h"
#include "Scene.h"
#include "MaterialManager.h"
#include "ResourceManager.h"

SpriteRenderer::SpriteRenderer(
    const String& texture_,
    int renderOrder_,
    const String& materialName)
    : RendererComponent(renderOrder_), materialName(materialName), textureName(texture_) {}

void SpriteRenderer::Awake()
{
    RendererComponent::Awake();

    material = gameObject->GetScene()->GetMaterialManager()->GetMaterial(materialName);
    texture = gameObject->GetScene()->GetResourceManager()->Get<Texture>(textureName);

    if (texture == nullptr) {
		texture = gameObject->GetScene()->GetResourceManager()->Get<Texture>("default_texture");
    }
}

void SpriteRenderer::Render()
{
    auto transform = gameObject->GetTransform()->GetFinalScreenTransform();
    const auto& renderer = GetRenderer();

    material->SetUniform("u_color", { 1.0f, 1.0f, 1.0f, 1.0f });
    material->SetUniform("u_pixel_top_left", transform.pos.to_vector());
    material->SetUniform("u_pixel_size", transform.size.to_vector());
    material->SetUniform("u_screen_size", { renderer->GetFW(), renderer->GetFH()});

    renderer->ApplyMaterial(material.get());
    renderer->ApplyTexture(texture);
    renderer->Draw(6);
}