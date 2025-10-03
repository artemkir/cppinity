// MaterialManager.cpp
#include "MaterialManager.h"
#include "ShaderManager.h"
#include <stdexcept>

MaterialManager::MaterialManager(ShaderManager* sm) : shaderManager(sm) {}

std::shared_ptr<Material> MaterialManager::CreateMaterial(const std::string& id,
                                                          std::shared_ptr<Shader> shader) {
    if (materials_.find(id) != materials_.end()) {
        throw std::runtime_error("Material already exists: " + id);
    }
    auto material = std::make_shared<Material>(shader);
    materials_[id] = material;
    return material;
}

std::shared_ptr<Material> MaterialManager::GetMaterial(const std::string& id) const {
    auto it = materials_.find(id);
    if (it == materials_.end()) {
        throw std::runtime_error("Material not found: " + id);
    }
    return it->second;
}

void MaterialManager::CreateDefaultMaterials()
{
    std::string vs = "#version 300 es\n"
                     "uniform vec2 u_pixel_top_left;\n"
                     "uniform vec2 u_pixel_size;\n"
                     "uniform vec2 u_screen_size;\n"
                     "uniform vec4 u_color;\n"
                     "in vec2 pos;\n"
                     "out vec4 color;\n"
                     "void main() {\n"
                     "  float pixel_x = u_pixel_top_left.x + pos.x * u_pixel_size.x;\n"
                     "  float pixel_y = u_pixel_top_left.y + (1.0 - pos.y) * u_pixel_size.y;\n"
                     "  float norm_x = (pixel_x / u_screen_size.x) * 2.0 - 1.0;\n"
                     "  float norm_y = 1.0 - (pixel_y / u_screen_size.y) * 2.0;\n"
                     "  gl_Position = vec4(norm_x, norm_y, 0.0, 1.0);\n"
                     "  color = u_color;\n"
                     "}\n";

    std::string fs = "#version 300 es\n"
                      "precision mediump float;\n"
                      "in vec4 color;\n"
                      "out vec4 frag_color;\n"
                      "void main() {\n"
                      "  frag_color = color;\n"
                      "}\n";

    std::vector<Shader::AttributeDesc> attrs = {
        {"pos", VertexFormat::FLOAT2}
    };

    std::vector<Shader::UniformDesc> unis = {
        {"u_pixel_top_left", UniformType::FLOAT2},
        {"u_pixel_size", UniformType::FLOAT2},
        {"u_screen_size", UniformType::FLOAT2},
        {"u_color", UniformType::FLOAT4}
    };
    Shader::UniformBlockDesc ub = {0, unis};

    std::vector<std::string> image_names;  // Empty for color rect

    auto shader = shaderManager->CreateShader("rect_shader", attrs, ub, image_names, vs, fs);

    CreateMaterial("rect_material", shader);
}