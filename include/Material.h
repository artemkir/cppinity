// Material.h
#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "RenderTypes.h"
#include "ITexture.h"  // Assume existing
#include "Shader.h"

class Material {
public:
    Material(std::shared_ptr<Shader> shader);

    ~Material();

    void SetUniform(const std::string& name, const std::vector<float>& value);

    void SetTexture(const std::string& name, std::shared_ptr<ITexture> texture);

    uint32_t GetPipelineID() const { return pipeline_id_; }

    const unsigned char* GetUniformData() const;

    int GetUniformSize() const { return shader_->GetVSUniformBlock().size; }

    std::shared_ptr<ITexture> GetTexture(const std::string& name) const;

    const Shader* GetShader() const { return shader_.get(); }

private:
    std::shared_ptr<Shader> shader_;
    uint32_t pipeline_id_ = 0;
    std::unordered_map<std::string, std::vector<float>> uniform_values_;
    std::unordered_map<std::string, std::shared_ptr<ITexture>> textures_;
    mutable std::vector<unsigned char> uniform_buffer_;
    mutable bool dirty_ = true;
};

#endif