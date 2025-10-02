// Texture.h
#pragma once

#include <cstdint>
#include "ITexture.h"

class Texture final : public ITexture
{
private:
    uint32_t texture;

public:
    explicit Texture(uint32_t tex);
    ~Texture() final;

    // Deleted copy constructor and assignment operator
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    // Move constructor and assignment operator
    Texture(Texture &&other) noexcept;
    Texture &operator=(Texture &&other) noexcept;

    uint32_t GetHandle() const noexcept;
};