
#include "Components/RectRenderer.h"

#include "GameObject.h"
#include "IRenderer.h"
#include "Components/Transform.h"
#include "MaterialManager.h"
#include "Scene.h"

RectRenderer::RectRenderer(
    unsigned char r_, 
    unsigned char g_, 
    unsigned char b_, 
    int renderOrder_,
    const String &materialName)
	: RendererComponent(renderOrder_), color{ r_, g_, b_ }, materialName(materialName)
{
    
}

void RectRenderer::Awake()
{
    RendererComponent::Awake();

    material = gameObject->GetScene()->GetMaterialManager()->GetMaterial(materialName);
}

RectRenderer::Color RectRenderer::GetColor()
{
    return color;
}

void RectRenderer::Render()
{
    auto transform = gameObject->GetTransform()->GetFinalScreenTransform();
    const auto& renderer = GetRenderer();

    material->SetUniform("u_color", {color.r / 255.f, color.g / 255.f, color.b / 255.f, 1.0f});
    material->SetUniform("u_pixel_top_left", transform.pos.to_vector());
    material->SetUniform("u_pixel_size", transform.size.to_vector());
    material->SetUniform("u_screen_size", {renderer->GetFW(), renderer->GetFH() });

    renderer->ApplyMaterial(material.get());
    renderer->ApplyTexture(nullptr);
    renderer->Draw(6);
}