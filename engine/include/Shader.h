#pragma once

#include "Std.h"
#include "RenderTypes.h"

class Shader {
public:
    struct AttributeDesc {
        String name;
        VertexFormat format;
    };

    struct UniformDesc {
        String name;
        UniformType type;
        int offset = 0;
    };

    struct UniformBlockDesc {
        int size = 0;
        Vector<UniformDesc> uniforms;
    };

    Shader(const Vector<AttributeDesc>& attrs,
           const UniformBlockDesc& vs_uniform_block,
           const Vector<String>& fs_image_names,
           const String& vs_source,
           const String& fs_source);

    ~Shader();

    uint32_t GetID() const { return id; }
    const Vector<AttributeDesc>& GetAttributes() const { return attrs; }
    const UniformBlockDesc& GetVSUniformBlock() const { return vsUniformBlock; }
    const UnorderedMapStringKey<int>& GetImageSlots() const { return imageSlots; }

private:
    uint32_t id = 0;
    Vector<AttributeDesc> attrs;
    UniformBlockDesc vsUniformBlock;
    Vector<String> fsImageNames;
    UnorderedMapStringKey<int> imageSlots;
    String vsSource;
    String fsSource;
};