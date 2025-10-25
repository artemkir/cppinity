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
        List<UniformDesc> uniforms;
    };

    explicit Shader(const List<AttributeDesc>& attrs,
           const UniformBlockDesc& vs_uniform_block,
           const List<String>& fs_image_names,
           const String& vs_source,
           const String& fs_source);

    ~Shader();
    Shader(const Shader&) = delete;
    Shader & operator=(const Shader&) = delete;
    Shader(Shader&&) noexcept;
    Shader & operator=(Shader&&) noexcept;

    uint32_t GetID() const { return id; }
    const List<AttributeDesc>& GetAttributes() const { return attrs; }
    const UniformBlockDesc& GetVSUniformBlock() const { return vsUniformBlock; }
    const UnorderedMapStringKey<int>& GetImageSlots() const { return imageSlots; }

private:
    uint32_t id = 0;
    List<AttributeDesc> attrs;
    UniformBlockDesc vsUniformBlock;
    List<String> fsImageNames;
    UnorderedMapStringKey<int> imageSlots;
    String vsSource;
    String fsSource;
};