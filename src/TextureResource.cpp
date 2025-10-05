#include "TextureResource.h"
#include "ResourceManager.h"
#include <cstring>  // for memcmp

TextureResource::TextureResource(ResourceManager* mgr)
    : Resource(mgr)
{
    if (!manager_->GetRenderer())
    {
        throw std::runtime_error("No IRenderer available in ResourceManager");
    }
}

std::shared_ptr<Texture> TextureResource::GetTexture() const
{
    if (state_ != ResourceState::Loaded)
    {
        throw std::runtime_error("Texture not loaded");
    }
    return texture_;
}

void TextureResource::ProcessData(const void* data, size_t size)
{
    manager_->GetRenderer()->UpdateTextureFromMemoryFile(texture_, data, size);
}