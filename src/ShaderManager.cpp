// ShaderManager.cpp
#include "ShaderManager.h"
#include <stdexcept>

std::shared_ptr<Shader> ShaderManager::CreateShader(const std::string& id,
                                                    const std::vector<Shader::AttributeDesc>& attrs,
                                                    const Shader::UniformBlockDesc& vs_uniform_block,
                                                    const std::vector<std::string>& fs_image_names,
                                                    const std::string& vs_source,
                                                    const std::string& fs_source) {
    if (shaders_.find(id) != shaders_.end()) {
        throw std::runtime_error("Shader already exists: " + id);
    }
    auto shader = std::make_shared<Shader>(attrs, vs_uniform_block, fs_image_names, vs_source, fs_source);
    shaders_[id] = shader;
    return shader;
}

std::shared_ptr<Shader> ShaderManager::GetShader(const std::string& id) const {
    auto it = shaders_.find(id);
    if (it == shaders_.end()) {
        throw std::runtime_error("Shader not found: " + id);
    }
    return it->second;
}