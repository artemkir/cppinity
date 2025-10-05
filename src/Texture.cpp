#include "Texture.h"
#include "ResourceManager.h"
#include "IRenderer.h"

Texture::Texture(ResourceManager* mgr) : Resource(mgr) {
    if (!manager_->GetRenderer()) {
        throw std::runtime_error("No IRenderer available in ResourceManager");
    }

	_data = manager_->GetRenderer()->PreallocateEmptyTexture();
}

Texture::~Texture() {
    manager_->GetRenderer()->DestroyTexture(_data);
}

Texture::Texture(Texture&& other) noexcept : _data(other._data) {
	other._data = { 0,0 };
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (_data.texture != 0) {
            manager_->GetRenderer()->DestroyTexture(_data);
        }
        _data = other._data;
        other._data = { 0,0 };
    }
    return *this;
}

void Texture::CreateFromFileData(const void* data, size_t size) {
    
    _data = manager_->GetRenderer()->CreateTextureFromMemoryFile(_data.view, data, size);
}

void Texture::CreateFromMemory(const void* data, size_t size) {

    throw std::runtime_error("Function isn't implemented");
}

void Texture::CreateTextureFromGrayscalePixelData(int width, int height, const void* data) {

    _data = manager_->GetRenderer()->CreateTextureFromGrayscalePixelData(_data.view, height, width, static_cast<const uint8_t*>(data));
}

void Texture::CreateRGBATextureFromPixelData(int width, int height, const void* data) {

    _data = manager_->GetRenderer()->CreateRGBATextureFromPixelData(_data.view, width, height, static_cast<const uint8_t*>(data));
}