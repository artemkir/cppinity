// Shader.cpp
#include "Shader.h"
#include <algorithm>  // For std::find_if

extern "C" uint32_t sokol_create_shader(
    const char* vs_source, 
    const char* fs_source,                                        
    int num_attrs, 
    const char** attr_names, 
    const int* attr_formats,
    int uniform_block_size,
    int num_uniforms,
    const char** uniform_names, 
    const int* uniform_types,
    int num_images, 
    const char** image_names);

extern "C" void sokol_destroy_shader(uint32_t id);

Shader::Shader(const std::vector<AttributeDesc>& _attrs,
               const UniformBlockDesc& _vs_uniform_block,
               const std::vector<std::string>& _fs_image_names,
               const std::string& _vs_source,
               const std::string& _fs_source)
    : attrs(_attrs), fsImageNames(_fs_image_names), vsSource(_vs_source), fsSource(_fs_source), vsUniformBlock(_vs_uniform_block) {
        
    int offset = 0;
    for (auto& u : vsUniformBlock.uniforms) {
        u.offset = offset;
        offset += RenderTypeUtils::GetSize(u.type);
    }
    vsUniformBlock.size = offset; 

    // Prepare attr arrays
    std::vector<const char*> attr_names_ptr(attrs.size());
    std::vector<int> attr_formats(attrs.size());
    for (size_t i = 0; i < attrs.size(); ++i) {
        attr_names_ptr[i] = attrs[i].name.c_str();
        attr_formats[i] = static_cast<int>(attrs[i].format);
    }

    // Prepare uniform arrays
    std::vector<const char*> uni_names_ptr(vsUniformBlock.uniforms.size());
    std::vector<int> uni_types(vsUniformBlock.uniforms.size());
    for (size_t i = 0; i < vsUniformBlock.uniforms.size(); ++i) {
        uni_names_ptr[i] = vsUniformBlock.uniforms[i].name.c_str();
        uni_types[i] = static_cast<int>(vsUniformBlock.uniforms[i].type);
    }

    // Prepare image names
    std::vector<const char*> image_names_ptr(fsImageNames.size());
    for (size_t i = 0; i < fsImageNames.size(); ++i) {
        image_names_ptr[i] = fsImageNames[i].c_str();
        imageSlots[fsImageNames[i]] = i;
    }

    // Create via wrapper
    id = sokol_create_shader(
        vsSource.c_str(), 
        fsSource.c_str(),
        attrs.size(), 
        attr_names_ptr.data(), 
        attr_formats.data(),
        vsUniformBlock.size, 
        vsUniformBlock.uniforms.size(),
        uni_names_ptr.data(), 
        uni_types.data(),
        fsImageNames.size(), 
        image_names_ptr.data()
    );
}

Shader::~Shader() {
    if (id != 0) {
        sokol_destroy_shader(id);
    }
}