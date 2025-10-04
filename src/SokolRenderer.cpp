#include "SokolRenderer.h"
#include "Material.h"

extern "C" uint32_t sokol_create_texture(int width, int height, const uint8_t* pixelData);
extern "C" void sokol_begin_pass();
extern "C" void sokol_end_pass();
extern "C" void sokol_setup();
extern "C" void sokol_draw_screen_quad(const float pos[2], const float size[2], const float color[4]);
extern "C" void sokol_apply_pipeline(uint32_t id);
extern "C" void sokol_apply_uniforms(const void* data, int size);
extern "C" void sokol_apply_view(uint32_t view_id);
extern "C" void sokol_draw(int num_elements);
extern "C" uint32_t sokol_create_view(uint32_t texture_id);

SokolRenderer::SokolRenderer() {
    sokol_setup();
}

void SokolRenderer::BeginPass() const {
    sokol_begin_pass();
}

void SokolRenderer::DrawRect(float x, float y, float w, float h, float col[4]) const {
    float pos[2] = { x,y };
    float size[2] = { w,h };

    sokol_draw_screen_quad(pos, size, col);
}

void SokolRenderer::EndPass() const {
    sokol_end_pass();
}

void SokolRenderer::ApplyMaterial(const Material* material) {
    //if (material != current_material_) {
        sokol_apply_pipeline(material->GetPipelineID());
    //    current_material_ = material;
    //    current_pipeline_ = material->GetPipelineID();
    //}

    // Always apply uniforms (may change per draw)
    sokol_apply_uniforms(material->GetUniformData(), material->GetUniformSize());
}

void SokolRenderer::ApplyTexture(const std::shared_ptr<ITexture>& texture) {
    if (texture) {
        const Texture& tex = dynamic_cast<const Texture&>(*texture);
        sokol_apply_view(tex.GetViewHandle());
    }
    else
    {
        sokol_apply_view(0);
    }
}

void SokolRenderer::Draw(int num_elements) {
    sokol_draw(num_elements);
}

std::shared_ptr<ITexture> SokolRenderer::CreateTexture(int width, int height, const unsigned char* pixelData) const {
    uint32_t id = sokol_create_texture(width, height, pixelData);
    uint32_t view = sokol_create_view(id);
    
    if (id == 0 || view == 0) {
        throw std::runtime_error("Failed to create texture");
    }
    
	return std::make_shared<Texture>(id, view);
}

void SokolRenderer::DrawTexture(float x, float y, float w, float h, const ITexture& texture) const {
    const Texture& tex = dynamic_cast<const Texture&>(texture);

}