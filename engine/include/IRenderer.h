#pragma once

#include "Std.h"

#include "Texture.h"

class Material;

class IRenderer
{
public:
	virtual ~IRenderer() = default;
	virtual void BeginPass() const = 0;
	virtual void DrawRect(float x, float y, float w, float h, float col[4]) const = 0;
	virtual void EndPass() const = 0;

	virtual int GetW() const = 0;
	virtual int GetH() const = 0;

	virtual float GetFW() const = 0;
	virtual float GetFH() const = 0;

	virtual TextureData CreateRGBATextureFromPixelData(uint32_t view, int width, int height, const uint8_t* pixelData) const = 0;
	virtual TextureData CreateTextureFromGrayscalePixelData(uint32_t view, int width, int height, const uint8_t* pixelData) const = 0;
	virtual TextureData CreateTextureFromMemoryFile(uint32_t view, const uint8_t* data, size_t size) const = 0;
	virtual TextureData PreallocateEmptyTexture() const = 0;
	virtual void DestroyTexture(const TextureData& texture) const = 0;

	virtual void ApplyMaterial(const Material* material) = 0;
    virtual void ApplyTexture(const SharedPtr<Texture>& texture) = 0;  // For simple single-texture case
    virtual void Draw(int num_elements) = 0;
};