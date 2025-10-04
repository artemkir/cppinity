// Texture.h
#pragma once

#include <cstdint>
#include "Texture.h"

class Texture /*final : public Texture*/
{
private:
    uint32_t texture;
    uint32_t view;

public:
    uint32_t GetTexture() const { return texture; }
    uint32_t GetView() const { return view; }
    void SetTexture(uint32_t t) { texture = t; }
    void SetView(uint32_t v) { view = v; }

    explicit Texture(uint32_t tex, uint32_t view);
    ~Texture();
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;
    Texture(Texture &&other) noexcept;
    Texture &operator=(Texture &&other) noexcept;
    void UpdateContent(int width, int height, const unsigned char* pixelData);
};