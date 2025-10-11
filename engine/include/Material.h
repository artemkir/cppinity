// Material.h
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Std.h"

#include "RenderTypes.h"
#include "Texture.h"  // Assume existing
#include "Shader.h"

class Material {
public:
    explicit Material(SharedPtr<Shader> shader);

    ~Material();

    void SetUniform(const String& name, const List<float>& value);

    void SetTexture(const String& name, SharedPtr<Texture> texture);

    uint32_t GetPipelineID() const { return pipeline_id_; }

    const uint8_t* GetUniformData() const;

    int GetUniformSize() const { return shader_->GetVSUniformBlock().size; }

    SharedPtr<Texture> GetTexture(const String& name) const;

    const Shader* GetShader() const { return shader_.get(); }

private:
    SharedPtr<Shader> shader_;
    uint32_t pipeline_id_ = 0;
    UnorderedMapStringKey<List<float>> uniform_values_;
    UnorderedMapStringKey<SharedPtr<Texture>> textures_;
    mutable List<unsigned char> uniform_buffer_;
    mutable bool dirty_ = true;
};

#endif