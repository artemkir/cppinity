// ShaderManager.h
#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Std.h"

#include "Shader.h"

class ShaderManager {
public:
    SharedPtr<Shader> CreateShader(const String& id,
                                         const Vector<Shader::AttributeDesc>& attrs,
                                         const Shader::UniformBlockDesc& vs_uniform_block,
                                         const Vector<String>& fs_image_names,
                                         const String& vs_source,
                                         const String& fs_source);

    SharedPtr<Shader> GetShader(const String& id) const;

    void CreateDefaultShaders();

private:
    UnorderedMapStringKey<SharedPtr<Shader>> shaders_;

    void CreateUnlitColorPixelRectShader();
    void CreateUnlitTextureScreenShader();
};

#endif