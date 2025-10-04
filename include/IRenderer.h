#pragma once

#include "memory"
#include "Texture.h"

class Material;

class IRenderer
{
public:
	virtual ~IRenderer() = default;
	virtual void BeginPass() const = 0;
	virtual void DrawRect(float x, float y, float w, float h, float col[4]) const = 0;
	virtual void EndPass() const = 0;

	virtual std::shared_ptr<Texture> CreateTextureFromGrayscalePixelData(int width, int height, const unsigned char *data) const = 0;
	virtual std::shared_ptr<Texture> CreateTextureFromMemoryFile(uint32_t view, const void* data, size_t size) const = 0;
	virtual std::shared_ptr<Texture> PreallocateEmptyTexture() const = 0;
	virtual void UpdateTextureFromMemoryFile(std::shared_ptr<Texture> tex, const void* data, size_t size) = 0;

	virtual void DrawTexture(float x, float y, float w, float h, const Texture &texture) const = 0;

	virtual void ApplyMaterial(const Material* material) = 0;
    virtual void ApplyTexture(const std::shared_ptr<Texture>& texture) = 0;  // For simple single-texture case
    virtual void Draw(int num_elements) = 0;
};