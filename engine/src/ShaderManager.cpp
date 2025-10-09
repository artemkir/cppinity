// ShaderManager.cpp
#include "ShaderManager.h"
#include <stdexcept>

SharedPtr<Shader> ShaderManager::CreateShader(const String& id,
                                                    const Vector<Shader::AttributeDesc>& attrs,
                                                    const Shader::UniformBlockDesc& vs_uniform_block,
                                                    const Vector<String>& fs_image_names,
                                                    const String& vs_source,
                                                    const String& fs_source) {
    if (shaders_.find(id) != shaders_.end()) {
        throw std::runtime_error("Shader already exists: " + id);
    }
    auto shader = MakeShared<Shader>(attrs, vs_uniform_block, fs_image_names, vs_source, fs_source);
    shaders_[id] = shader;
    return shader;
}

SharedPtr<Shader> ShaderManager::GetShader(const String& id) const {
    auto it = shaders_.find(id);
    if (it == shaders_.end()) {
        throw std::runtime_error("Shader not found: " + id);
    }
    return it->second;
}

void ShaderManager::CreateDefaultShaders()
{
    CreateUnlitColorPixelRectShader();
    CreateUnlitTextureScreenShader();
}

void ShaderManager::CreateUnlitColorPixelRectShader()
{
    String vs = 
        "#version 300 es\n"
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

    String fs = 
        "#version 300 es\n"
        "precision mediump float;\n"
        "in vec4 color;\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "  frag_color = color;\n"
        "}\n";

    Vector<Shader::AttributeDesc> attrs = {
        {"pos", VertexFormat::FLOAT2}
    };

    Vector<Shader::UniformDesc> unis = {
        {"u_pixel_top_left", UniformType::FLOAT2},
        {"u_pixel_size", UniformType::FLOAT2},
        {"u_screen_size", UniformType::FLOAT2},
        {"u_color", UniformType::FLOAT4}
    };
    Shader::UniformBlockDesc ub = { 0, unis };

    Vector<String> image_names;  // Empty for color rect

    CreateShader("unlit_color_pixel", attrs, ub, image_names, vs, fs);
}

void ShaderManager::CreateUnlitTextureScreenShader()
{
    String vs = 
        "#version 300 es\n"
        "uniform vec2 u_pixel_top_left;\n"
        "uniform vec2 u_pixel_size;\n"
        "uniform vec2 u_screen_size;\n"
        "uniform vec4 u_color;\n"
        "in vec2 pos;\n"
        //"in vec2 texcoord0;\n"
        "out vec4 color;\n"
        "out vec2 uv;"
        "void main() {\n"
        "  float pixel_x = u_pixel_top_left.x + pos.x * u_pixel_size.x;\n"
        "  float pixel_y = u_pixel_top_left.y + (1.0 - pos.y) * u_pixel_size.y;\n"
        "  float norm_x = (pixel_x / u_screen_size.x) * 2.0 - 1.0;\n"
        "  float norm_y = 1.0 - (pixel_y / u_screen_size.y) * 2.0;\n"
        "  gl_Position = vec4(norm_x, norm_y, 0.0, 1.0);\n"
        "  color = u_color;\n"
        "  uv = vec2(pos.x, 1.0 - pos.y);\n"
        "}\n";

    String fs = 
        "#version 300 es\n"
        "precision mediump float;\n"
        "uniform sampler2D tex;\n"
        "in vec4 color;\n"
        "in vec2 uv;\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "  frag_color = texture(tex, uv) * color;\n"
        "}\n";

    Vector<Shader::AttributeDesc> attrs = {
        {"pos", VertexFormat::FLOAT2},
        //{"texcoord0", VertexFormat::FLOAT2}
    };

    Vector<Shader::UniformDesc> unis = {
        {"u_pixel_top_left", UniformType::FLOAT2},
        {"u_pixel_size", UniformType::FLOAT2},
        {"u_screen_size", UniformType::FLOAT2},
        {"u_color", UniformType::FLOAT4}
    };
    Shader::UniformBlockDesc ub = { 0, unis };

    Vector<String> image_names = {
        "tex"
    };  

    CreateShader("unlit_texture_pixel", attrs, ub, image_names, vs, fs);
}
