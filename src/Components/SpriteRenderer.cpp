#include "Components/SpriteRenderer.h"
#include "GameObject.h"
#include "Components/Transform.h"
#include "IRenderer.h"

SpriteRenderer::SpriteRenderer(std::shared_ptr<ITexture> texture_, int renderOrder_)
    : RendererComponent(renderOrder_), texture(std::move(texture_)) {}

void SpriteRenderer::Render() {
    auto t = gameObject->GetTransform()->GetScreenTransform();
    const auto& renderer = GetRenderer();

    renderer->DrawTexture(t.x, t.y, t.width, t.height, *texture);
}