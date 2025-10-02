// Renderer.cpp (replaced SDLRenderer.cpp)
#include "Renderer.h"
//#include "sokol_gfx.h"
//#include "sokol_glue.h"
//#include "sokol_app.h"  // For sapp_width, sapp_height

Renderer::Renderer() {
    pass_action = {};
    pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
    pass_action.colors[0].clear_value = {0.0f, 0.0f, 0.0f, 1.0f};
}

void Renderer::Clear() const {
    /*sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());
    sgl_defaults();
    sgl_matrix_mode_projection();
    sgl_ortho(0.0f, (float)sapp_width(), (float)sapp_height(), 0.0f, -1.0f, 1.0f);*/
}

void Renderer::DrawRect(float x, float y, float w, float h) const {
    /*sgl_begin_quads();
    sgl_c4b(current_r, current_g, current_b, current_a);
    sgl_v2f(x, y);
    sgl_v2f(x + w, y);
    sgl_v2f(x + w, y + h);
    sgl_v2f(x, y + h);
    sgl_end();*/
}

void Renderer::SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) const {
    /*current_r = r;
    current_g = g;
    current_b = b;
    current_a = a;*/
}

void Renderer::Present() const {
    /*sgl_draw();
    sg_end_pass();
    sg_commit();*/
}

std::shared_ptr<ITexture> Renderer::CreateTexture(int width, int height, const unsigned char* pixelData) const {
    std::vector<uint32_t> rgbaPixels(width * height);
    for (int i = 0; i < width * height; ++i) {
        uint8_t value = pixelData[i];
        rgbaPixels[i] = value | (value << 8) | (value << 16) | (value == 0 ? 0 : 255 << 24);  // ABGR? Adjust if needed for Sokol
    }

    sg_image_desc desc = {};
    /*desc.width = width;
    desc.height = height;
    desc.pixel_format = SG_PIXELFORMAT_RGBA8;
    desc.min_filter = SG_FILTER_NEAREST;
    desc.mag_filter = SG_FILTER_NEAREST;
    desc.content.subimage[0][0].ptr = rgbaPixels.data();
    desc.content.subimage[0][0].size = width * height * 4;*/

    sg_image img = sg_make_image(&desc);
    if (sg_query_image_state(img) != SG_RESOURCESTATE_VALID) {
        throw std::runtime_error("Failed to create texture");
    }
    
    return std::make_shared<Texture>(img);
}

void Renderer::DrawTexture(float x, float y, float w, float h, const ITexture& texture) const {
    const Texture& tex = dynamic_cast<const Texture&>(texture);
    /*sgl_texture(tex.GetSGImage());
    sgl_begin_quads();
    sgl_c4b(255, 255, 255, 255);
    sgl_v2f_t2f(x, y, 0.0f, 0.0f);
    sgl_v2f_t2f(x + w, y, 1.0f, 0.0f);
    sgl_v2f_t2f(x + w, y + h, 1.0f, 1.0f);
    sgl_v2f_t2f(x, y + h, 0.0f, 1.0f);
    sgl_end();
    sgl_texture(sg_make_image(&(sg_image_desc){}));*/
}