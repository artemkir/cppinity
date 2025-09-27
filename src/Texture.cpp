#include "Texture.h"

Texture::Texture(SDL_Texture* tex) : texture(tex) {}

Texture::~Texture() {
    SDL_DestroyTexture(texture);
}

Texture::Texture(Texture&& other) noexcept : texture(other.texture) {
    other.texture = nullptr;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
        texture = other.texture;
        other.texture = nullptr;
    }
    return *this;
}

SDL_Texture* Texture::GetSDLTexture() const noexcept {
    return texture;
}