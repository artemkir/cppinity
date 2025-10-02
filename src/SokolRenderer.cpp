#include "SokolRenderer.h"

extern "C" uint32_t sokol_create_texture(int width, int height, const unsigned char* pixelData);
extern "C" void sokol_begin_pass();
extern "C" void sokol_end_pass();
extern "C" void sokol_setup();

SokolRenderer::SokolRenderer() {
    sokol_setup();
}

void SokolRenderer::BeginPass() const {
    
    sokol_begin_pass();
}

void SokolRenderer::DrawRect(float x, float y, float w, float h) const {

}

void SokolRenderer::SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) const {

}

void SokolRenderer::EndPass() const {

    sokol_end_pass();
}

std::shared_ptr<ITexture> SokolRenderer::CreateTexture(int width, int height, const unsigned char* pixelData) const {
    uint32_t id = sokol_create_texture(width, height, pixelData);
    
    //if (id == 0) {
    //    throw std::runtime_error("Failed to create texture");
    // }
    
	return std::make_shared<Texture>(id);
}

void SokolRenderer::DrawTexture(float x, float y, float w, float h, const ITexture& texture) const {
    const Texture& tex = dynamic_cast<const Texture&>(texture);

}