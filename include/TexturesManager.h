#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "ITexture.h"

class IRenderer;

class TexturesManager {
    IRenderer* renderer;
    std::unordered_map<std::string, std::shared_ptr<ITexture>> textures;

public:
    explicit TexturesManager(IRenderer* renderer_);

    std::shared_ptr<ITexture> LoadTexture(const std::string& id, int width, int height, const unsigned char* data);
    std::shared_ptr<ITexture> GetTexture(const std::string& id) const;
};