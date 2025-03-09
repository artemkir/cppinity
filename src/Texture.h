#pragma once

class Texture : public ITexture
{
	SDL_Texture* texture = nullptr;

public:

	explicit Texture(SDL_Texture* tex) : texture(tex) {}

	~Texture() final { SDL_DestroyTexture(texture);	}

    // Deleted copy constructor and assignment operator to prevent accidental resource duplication.
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Implemented move constructor and assignment operator to safely transfer SDL_Texture ownership.
    Texture(Texture&& other) noexcept : texture(other.texture) { other.texture = nullptr; }

    Texture& operator=(Texture&& other) noexcept {
        if (this != &other) {
            if (texture) {
                SDL_DestroyTexture(texture);
            }
            texture = other.texture;
            other.texture = nullptr;
        }
        return *this;
    }

	SDL_Texture* GetSDLTexture() const noexcept { return texture; }

};