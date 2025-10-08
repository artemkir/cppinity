// Renderer.h
#pragma once

#include "Std.h"

#include "IRenderer.h"
#include "Texture.h"

class SokolRenderer : public IRenderer
{
    const Material* current_material_ = nullptr;
    uint32_t current_pipeline_ = 0;
	int screen_width_;
	int screen_height_;

public:
    SokolRenderer(int, int);

    void BeginPass() const override;
    void DrawRect(float x, float y, float w, float h, float col[4]) const override;
    void EndPass() const override;

	int GetW() const override { return screen_width_; }
	int GetH() const override { return screen_height_; }

	TextureData CreateTextureFromGrayscalePixelData(uint32_t view, int width, int height, const uint8_t* pixelData) const override;
    TextureData CreateRGBATextureFromPixelData(uint32_t view, int width, int height, const uint8_t* pixelData) const override;
    TextureData CreateTextureFromMemoryFile(uint32_t view, const uint8_t* data, size_t size) const override;
    TextureData PreallocateEmptyTexture() const override;
	void DestroyTexture(const TextureData& data) const override;
        
    void ApplyMaterial(const Material* material) override;
    void ApplyTexture(const SharedPtr<Texture>& texture) override;
    void Draw(int num_elements) override;
};