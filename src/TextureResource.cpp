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
    const unsigned char* bytes = static_cast<const unsigned char*>(data);
    bool is_png = (size >= 8) && (memcmp(bytes, "\x89PNG\r\n\x1A\n", 8) == 0);
      
    if (!is_png)
    {
        throw std::runtime_error("Unsupported format; only PNG supported");
    }

    manager_->GetRenderer()->UpdateTextureFromMemoryFile(texture_, bytes, size);

    if (!texture_) {
        throw std::runtime_error("Failed to create texture");
    }
}