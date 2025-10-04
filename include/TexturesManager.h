// TexturesManager.h
#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "Texture.h"

class IRenderer;
class ResourceManager;

class TexturesManager
{
    IRenderer *renderer;
    ResourceManager* resourceManager;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

public:
    explicit TexturesManager(IRenderer *renderer_, ResourceManager* resourceManager_);

    std::shared_ptr<Texture> CreateTexture(const std::string &id, int width, int height, const uint8_t* data);
    std::shared_ptr<Texture> LoadFromFile(const std::string& id, const std::string& path);
    std::shared_ptr<Texture> GetTexture(const std::string &id) const;
};