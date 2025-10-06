// MaterialManager.h
#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <string>
#include <memory>
#include <unordered_map>
#include "Material.h"
#include "RenderTypes.h"
#include "StringHash.h"

class ShaderManager;

class MaterialManager {
public:
    explicit MaterialManager(ShaderManager *sm);

    std::shared_ptr<Material> CreateMaterial(const std::string& id,
                                             std::shared_ptr<Shader> shader);

    std::shared_ptr<Material> GetMaterial(const std::string& id) const;
        
    void CreateDefaultMaterials();

private:
    ShaderManager* shaderManager;
    std::unordered_map<std::string, std::shared_ptr<Material>, StringHash, std::equal_to<>> materials_;
};

#endif