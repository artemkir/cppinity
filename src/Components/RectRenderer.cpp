
#include "Components/RectRenderer.h"

#include "GameObject.h"
#include "IRenderer.h"
#include "Components/Transform.h"

RectRenderer::RectRenderer(unsigned char r_, unsigned char g_, unsigned char b_, int renderOrder_)
    : RendererComponent(renderOrder_), color{ r_, g_, b_ } {}

RectRenderer::Color RectRenderer::GetColor() {
    return color;
}

void RectRenderer::Render() {
    auto t = gameObject->GetTransform()->GetScreenTransform();
    const auto& renderer = GetRenderer();

    renderer->SetDrawColor(color.r, color.g, color.b, 255);
    renderer->DrawRect(t.x, t.y, t.width, t.height);
}