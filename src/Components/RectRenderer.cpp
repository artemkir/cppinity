
#include "Components/RectRenderer.h"

#include "GameObject.h"
#include "IRenderer.h"
#include "Components/Transform.h"
#include "MaterialManager.h"
#include "Scene.h"

extern "C" int sokol_get_screen_width(); 
extern "C" int sokol_get_screen_height();

RectRenderer::RectRenderer(
    unsigned char r_, 
    unsigned char g_, 
    unsigned char b_, 
    int renderOrder_,
    const std::string &materialName)
	: RendererComponent(renderOrder_), color{ r_, g_, b_ }, materialName(materialName)
{
    
}

void RectRenderer::Awake()
{
    material = gameObject->GetScene()->GetMaterialManager()->GetMaterial(materialName);
}

RectRenderer::Color RectRenderer::GetColor()
{
    return color;
}

void RectRenderer::Render()
{
    auto transform = gameObject->GetTransform()->GetScreenTransform();
    const auto& renderer = GetRenderer();

    material->SetUniform("u_color", {color.r / 255.f, color.g / 255.f, color.b / 255.f, 1.0f});
    material->SetUniform("u_pixel_top_left", {transform.x, transform.y});
    material->SetUniform("u_pixel_size", {transform.width, transform.height});
    auto sw = static_cast<float>(sokol_get_screen_width());
    auto sh = static_cast<float>(sokol_get_screen_height());
    material->SetUniform("u_screen_size", {sw, sh});

    renderer->ApplyMaterial(material.get());
    renderer->ApplyTexture(nullptr);
    renderer->Draw(6);
}