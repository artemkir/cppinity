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

    std::shared_ptr<Texture> CreateTextureFromGrayscalePixelData(int width, int height, const unsigned char *pixelData) const override;
    std::shared_ptr<Texture> CreateTextureFromMemoryFile(uint32_t view, const void* data, size_t size) const;
    std::shared_ptr<Texture> PreallocateEmptyTexture() const;
    void UpdateTextureFromMemoryFile(std::shared_ptr<Texture> tex, const void* data, size_t size);

    void DrawTexture(float x, float y, float w, float h, const Texture &texture) const override;

    void ApplyMaterial(const Material* material) override;
    void ApplyTexture(const std::shared_ptr<Texture>& texture) override;
    void Draw(int num_elements) override;
};