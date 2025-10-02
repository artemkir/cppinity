// Renderer.h
#pragma once

#include "IRenderer.h"
#include "Texture.h"
#include <vector>
#include <memory>
#include <stdexcept>

class SokolRenderer : public IRenderer
{
public:
    SokolRenderer();

    void BeginPass() const override;
    void DrawRect(float x, float y, float w, float h, float col[4]) const override;
    void EndPass() const override;
    std::shared_ptr<ITexture> CreateTexture(int width, int height, const unsigned char *pixelData) const override;
    void DrawTexture(float x, float y, float w, float h, const ITexture &texture) const override;
};