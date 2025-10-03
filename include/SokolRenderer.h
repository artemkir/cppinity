// Renderer.h
#pragma once

#include <vector>
#include <memory>
#include <stdexcept>

#include "IRenderer.h"
#include "Texture.h"

class SokolRenderer : public IRenderer
{
    const Material* current_material_ = nullptr;
    uint32_t current_pipeline_ = 0;

public:
    SokolRenderer();

    void BeginPass() const override;
    void DrawRect(float x, float y, float w, float h, float col[4]) const override;
    void EndPass() const override;
    std::shared_ptr<ITexture> CreateTexture(int width, int height, const unsigned char *pixelData) const override;
    void DrawTexture(float x, float y, float w, float h, const ITexture &texture) const override;

    void ApplyMaterial(const Material* material) override;
    void ApplyTexture(const std::shared_ptr<ITexture>& texture) override;
    void Draw(int num_elements) override;
};