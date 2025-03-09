#pragma once

#include "Texture.h"

// SDL Renderer Implementation
class SDLRenderer : public IRenderer {
private:
	SDL_Renderer* renderer;

public:
	SDLRenderer(SDL_Renderer* r) : renderer(r) {}

	void Clear() const override {
		SDL_RenderClear(renderer);
	}

	void DrawRect(float x, float y, float w, float h) const override {
		SDL_FRect rect = { x, y, w, h };
		SDL_RenderFillRectF(renderer, &rect);
	}

	void SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) const override {
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}

	void Present() const override {
		SDL_RenderPresent(renderer);
	}

	std::shared_ptr<ITexture> CreateTexture(int width, int height, const unsigned char* pixelData) const override
	{
		std::vector<Uint32> rgbaPixels(width * height);

		SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
				
		for (int i = 0; i < width * height; ++i) {
			Uint8 value = pixelData[i];
			rgbaPixels[i] = SDL_MapRGBA(format, value, value, value, value == 0 ? 0 : 255);
		}

		SDL_FreeFormat(format);

		// Create SDL texture
		SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, width, height);
		if (!texture) {
			throw std::runtime_error("Failed to create texture");
		}

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		// Update texture with pixel data
		if (SDL_UpdateTexture(texture, nullptr, rgbaPixels.data(), width * sizeof(Uint32)) != 0) {
			SDL_DestroyTexture(texture);
			throw std::runtime_error("Failed to create texture");
		}
		
		return std::make_shared<Texture>(texture);
	}
		
	void DrawTexture(float x, float y, float w, float h, const ITexture& texture) const override {
		const Texture& tex = dynamic_cast<const Texture&>(texture);

		SDL_FRect dstRect = { x, y, w, h };
		SDL_RenderCopyF(renderer, tex.GetSDLTexture(), nullptr, &dstRect);
	}
};