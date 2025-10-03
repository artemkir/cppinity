// ShaderManager.h
#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <string>
#include <memory>
#include <unordered_map>
#include "Shader.h"

class ShaderManager {
public:
    std::shared_ptr<Shader> CreateShader(const std::string& id,
                                         const std::vector<Shader::AttributeDesc>& attrs,
                                         const Shader::UniformBlockDesc& vs_uniform_block,
                                         const std::vector<std::string>& fs_image_names,
                                         const std::string& vs_source,
                                         const std::string& fs_source);

    std::shared_ptr<Shader> GetShader(const std::string& id) const;

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders_;
};

#endif