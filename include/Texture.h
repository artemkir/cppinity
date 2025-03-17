#pragma once

#include "ITexture.h"
#include <SDL.h>

class Texture : public ITexture {
private:
    SDL_Texture* texture = nullptr;

public:
    explicit Texture(SDL_Texture* tex);
    ~Texture() final;

    // Deleted copy constructor and assignment operator
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Move constructor and assignment operator
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    SDL_Texture* GetSDLTexture() const noexcept;
};
