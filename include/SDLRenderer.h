#pragma once

#include "IRenderer.h"
#include "Texture.h"
#include <SDL.h>
#include <vector>
#include <memory>
#include <stdexcept>

// SDL Renderer Implementation
class SDLRenderer : public IRenderer {
private:
    SDL_Renderer* renderer;

public:
    SDLRenderer(SDL_Renderer* r);

    void Clear() const override;
    void DrawRect(float x, float y, float w, float h) const override;
    void SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) const override;
    void Present() const override;
    std::shared_ptr<ITexture> CreateTexture(int width, int height, const unsigned char* pixelData) const override;
    void DrawTexture(float x, float y, float w, float h, const ITexture& texture) const override;
};
