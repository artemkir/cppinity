// Texture.h
#pragma once

#include <cstdint>
#include "ITexture.h"

class Texture final : public ITexture
{
private:
    uint32_t texture;
    uint32_t view;

public:
    explicit Texture(uint32_t tex, uint32_t view);
    ~Texture() final;
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;
    Texture(Texture &&other) noexcept;
    Texture &operator=(Texture &&other) noexcept;
    uint32_t GetViewHandle() const noexcept;
};