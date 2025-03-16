#pragma once

class TexturesManager {
    IRenderer& renderer;
    std::unordered_map<std::string, std::shared_ptr<ITexture>> textures;

public:
    explicit TexturesManager(IRenderer& renderer_) : renderer(renderer_) {}

    std::shared_ptr<ITexture> LoadTexture(const std::string& id, int width, int height, const unsigned char* data) {
        auto it = textures.find(id);
        if (it != textures.end()) {
            return it->second;
        }

        auto texture = renderer.CreateTexture(width, height, data);
        textures.emplace(id, texture);
        return texture;
    }

    std::shared_ptr<ITexture> GetTexture(const std::string& id) const {
        auto it = textures.find(id);
        if (it != textures.end()) {
            return it->second;
        }
        return nullptr;
    }
};