#include "Texture.h"

extern "C" void sokol_destroy_texture(uint32_t id);

Texture::Texture(uint32_t tex) : texture(tex) {}

Texture::~Texture() {
    sokol_destroy_texture(texture);
}

Texture::Texture(Texture&& other) noexcept : texture(other.texture) {
    other.texture = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (texture != 0) {
            sokol_destroy_texture(texture);
        }
        texture = other.texture;
        other.texture = 0;
    }
    return *this;
}

uint32_t Texture::GetHandle() const noexcept {
    return texture;
}