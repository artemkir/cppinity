// TexturesManager.cpp (unchanged, no SDL dependencies)
#include "TexturesManager.h"
#include "IRenderer.h"

TexturesManager::TexturesManager(IRenderer* renderer_) : renderer(renderer_) {}

std::shared_ptr<ITexture> TexturesManager::LoadTexture(const std::string& id, int width, int height, const unsigned char* data) {
    auto it = textures.find(id);
    if (it != textures.end()) {
        return it->second;
    }

    auto texture = renderer->CreateTexture(width, height, data);
    textures.emplace(id, texture);
    return texture;
}

std::shared_ptr<ITexture> TexturesManager::GetTexture(const std::string& id) const {
    auto it = textures.find(id);
    if (it != textures.end()) {
        return it->second;
    }
    return nullptr;
}