// TexturesManager.cpp (unchanged, no SDL dependencies)
#include "TexturesManager.h"
#include "TextureResource.h"
#include "ResourceManager.h"
#include "IRenderer.h"

TexturesManager::TexturesManager(IRenderer* renderer_, ResourceManager* resourceManager_) : 
    renderer(renderer_), resourceManager(resourceManager_) {}

std::shared_ptr<Texture> TexturesManager::CreateTexture(const std::string& id, int width, int height, const uint8_t* data) {
    auto it = textures.find(id);
    if (it != textures.end()) {
        return it->second;
    }

    auto texture = renderer->CreateTextureFromGrayscalePixelData(width, height, data);
    textures.emplace(id, texture);
    return texture;
}

std::shared_ptr<Texture> TexturesManager::LoadFromFile(const std::string& id, const std::string& path) {
    auto it = textures.find(id);
    if (it != textures.end()) {
        return it->second;
    }

    auto placeholder = renderer->PreallocateEmptyTexture();//renderer->CreateTexture(1, 1, dummy);
    
    if (!placeholder) {
        // Handle failure, e.g., return nullptr or throw
        return nullptr;
    }

    textures.emplace(id, placeholder);
        
    auto resource = resourceManager->Load<TextureResource>(path);

    resource->SetTarget(placeholder);

    return placeholder;
}

std::shared_ptr<Texture> TexturesManager::GetTexture(const std::string& id) const {
    auto it = textures.find(id);
    if (it != textures.end()) {
        return it->second;
    }
    return nullptr;
}