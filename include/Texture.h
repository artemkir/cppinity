// Texture.h
#pragma once

#include "ITexture.h"
//#include "sokol_gfx.h"

class Texture : public ITexture {
private:
    sg_image texture;

public:
    explicit Texture(sg_image tex);
    ~Texture() final;

    // Deleted copy constructor and assignment operator
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Move constructor and assignment operator
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    sg_image GetSGImage() const noexcept;
};