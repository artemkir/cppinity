// Texture.h
#pragma once

#include <cstdint>
#include "Texture.h"
#include "Resource.h"

struct TextureData {
    uint32_t texture;
    uint32_t view;
};

class Texture : public Resource
{
private:
	TextureData _data{0,0};

public:
    uint32_t GetTexture() const { return _data.texture; }
    uint32_t GetView() const { return _data.view; }
    void SetTexture(uint32_t t) { _data.texture = t; }
    void SetView(uint32_t v) { _data.view = v; }

    explicit Texture(ResourceManager* mgr);
    ~Texture() final;
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;
    Texture(Texture &&other) noexcept;
    Texture &operator=(Texture &&other) noexcept;

    friend class ResourceManager;

    void CreateTextureFromGrayscalePixelData(int width, int height, const uint8_t* data);
    void CreateRGBATextureFromPixelData(int width, int height, const uint8_t* data);

protected:
    void CreateFromFileData(const uint8_t* data, size_t size) override;
    void CreateFromMemory(const uint8_t* data, size_t size) override;
    
};