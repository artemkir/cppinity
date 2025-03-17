#include "SDLRenderer.h"

SDLRenderer::SDLRenderer(SDL_Renderer* r) : renderer(r) {}

void SDLRenderer::Clear() const {
    SDL_RenderClear(renderer);
}

void SDLRenderer::DrawRect(float x, float y, float w, float h) const {
    SDL_FRect rect = { x, y, w, h };
    SDL_RenderFillRectF(renderer, &rect);
}

void SDLRenderer::SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) const {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void SDLRenderer::Present() const {
    SDL_RenderPresent(renderer);
}

std::shared_ptr<ITexture> SDLRenderer::CreateTexture(int width, int height, const unsigned char* pixelData) const {
    std::vector<Uint32> rgbaPixels(width * height);
    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    
    for (int i = 0; i < width * height; ++i) {
        Uint8 value = pixelData[i];
        rgbaPixels[i] = SDL_MapRGBA(format, value, value, value, value == 0 ? 0 : 255);
    }

    SDL_FreeFormat(format);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, width, height);
    if (!texture) {
        throw std::runtime_error("Failed to create texture");
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    if (SDL_UpdateTexture(texture, nullptr, rgbaPixels.data(), width * sizeof(Uint32)) != 0) {
        SDL_DestroyTexture(texture);
        throw std::runtime_error("Failed to create texture");
    }
    
    return std::make_shared<Texture>(texture);
}

void SDLRenderer::DrawTexture(float x, float y, float w, float h, const ITexture& texture) const {
    const Texture& tex = dynamic_cast<const Texture&>(texture);
    SDL_FRect dstRect = { x, y, w, h };
    SDL_RenderCopyF(renderer, tex.GetSDLTexture(), nullptr, &dstRect);
}