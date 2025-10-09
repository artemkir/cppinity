// MaterialManager.h
#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include "Std.h"

#include "Material.h"
#include "RenderTypes.h"

class ShaderManager;

class MaterialManager {
public:
    explicit MaterialManager(ShaderManager *sm);

    SharedPtr<Material> CreateMaterial(const String& id,
                                             SharedPtr<Shader> shader);

    SharedPtr<Material> GetMaterial(const String& id) const;
        
    void CreateDefaultMaterials();

private:
    ShaderManager* shaderManager;
    UnorderedMapStringKey<SharedPtr<Material>> materials_;
};

#endif