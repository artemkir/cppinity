#pragma once

class Texture : public ITexture
{
	SDL_Texture* texture;

public:

	explicit Texture(SDL_Texture* tex) : texture(tex) {}

	~Texture() final {
		if (texture) {
			SDL_DestroyTexture(texture);
		}
	}

	SDL_Texture* GetSDLTexture() const { return texture; }

};