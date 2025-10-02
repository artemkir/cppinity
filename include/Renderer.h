// Renderer.h
#pragma once

#include "IRenderer.h"
#include "Texture.h"
#include <vector>
#include <memory>
#include <stdexcept>

#define SOKOL_GLCORE
#define SOKOL_IMPL
#include "sokol_gfx.h"


// Sokol Renderer Implementation
class Renderer : public IRenderer {
private:
    unsigned char current_r = 0, current_g = 0, current_b = 0, current_a = 255;
    sg_pass_action pass_action;

public:
    Renderer();

    void Clear() const override;
    void DrawRect(float x, float y, float w, float h) const override;
    void SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) const override;
    void Present() const override;
    std::shared_ptr<ITexture> CreateTexture(int width, int height, const unsigned char* pixelData) const override;
    void DrawTexture(float x, float y, float w, float h, const ITexture& texture) const override;
};