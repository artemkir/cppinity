#include "Texture.h"

extern "C" void sokol_destroy_texture(uint32_t id);
extern "C" void sokol_destroy_view(uint32_t id);

Texture::Texture(uint32_t tex, uint32_t view) : texture(tex), view(view) {}

Texture::~Texture() {
    sokol_destroy_texture(texture);
    sokol_destroy_view(view);
}

Texture::Texture(Texture&& other) noexcept : texture(other.texture), view(other.view) {
    other.texture = 0;
    other.view = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (texture != 0) {
            sokol_destroy_texture(texture);
            sokol_destroy_view(view);
        }
        texture = other.texture;
        view = other.view;
        other.texture = 0;
        other.view = 0;
    }
    return *this;
}

void Texture::UpdateContent(int width, int height, const unsigned char* pixelData) {
    if (texture != 0) {
        sokol_destroy_texture(texture);
    }
    //texture = sokol_create_texture(width, height, pixelData);
    // Assume sokol_create_texture handles failure by returning 0; you can add check/throw if needed
}