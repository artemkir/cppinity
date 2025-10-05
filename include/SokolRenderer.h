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

	TextureData CreateTextureFromGrayscalePixelData(uint32_t view, int width, int height, const unsigned char* pixelData) const override;
    TextureData CreateRGBATextureFromPixelData(uint32_t view, int width, int height, const unsigned char* pixelData) const override;
    TextureData CreateTextureFromMemoryFile(uint32_t view, const void* data, size_t size) const override;
    TextureData PreallocateEmptyTexture() const override;
	void DestroyTexture(const TextureData& data) const override;
        
    void ApplyMaterial(const Material* material) override;
    void ApplyTexture(const std::shared_ptr<Texture>& texture) override;
    void Draw(int num_elements) override;
};