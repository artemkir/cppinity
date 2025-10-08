// MaterialManager.cpp
#include "MaterialManager.h"
#include "ShaderManager.h"
#include <stdexcept>

MaterialManager::MaterialManager(ShaderManager* sm) : shaderManager(sm) {}

SharedPtr<Material> MaterialManager::CreateMaterial(const String& id,
                                                          SharedPtr<Shader> shader) {
    if (materials_.find(id) != materials_.end()) {
        throw std::runtime_error("Material already exists: " + id);
    }
    auto material = std::make_shared<Material>(shader);
    materials_[id] = material;
    return material;
}

SharedPtr<Material> MaterialManager::GetMaterial(const String& id) const {
    auto it = materials_.find(id);
    if (it == materials_.end()) {
        throw std::runtime_error("Material not found: " + id);
    }
    return it->second;
}

void MaterialManager::CreateDefaultMaterials()
{
    CreateMaterial("UI/Color", shaderManager->GetShader("unlit_color_pixel"));
    CreateMaterial("UI/Texture", shaderManager->GetShader("unlit_texture_pixel"));
}