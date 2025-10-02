// Texture.cpp (updated for sg_image)
#include "Texture.h"
//#include "sokol_gfx.h"

Texture::Texture(sg_image tex) : texture(tex) {}

Texture::~Texture() {
    sg_destroy_image(texture);
}

Texture::Texture(Texture&& other) noexcept : texture(other.texture) {
    other.texture.id = SG_INVALID_ID;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (texture.id != SG_INVALID_ID) {
            sg_destroy_image(texture);
        }
        texture = other.texture;
        other.texture.id = SG_INVALID_ID;
    }
    return *this;
}

sg_image Texture::GetSGImage() const noexcept {
    return texture;
}