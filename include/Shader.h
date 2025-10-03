#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "RenderTypes.h"

class Shader {
public:
    struct AttributeDesc {
        std::string name;
        VertexFormat format;
    };

    struct UniformDesc {
        std::string name;
        UniformType type;
        int offset = 0;
    };

    struct UniformBlockDesc {
        int size = 0;
        std::vector<UniformDesc> uniforms;
    };

    Shader(const std::vector<AttributeDesc>& attrs,
           const UniformBlockDesc& vs_uniform_block,
           const std::vector<std::string>& fs_image_names,
           const std::string& vs_source,
           const std::string& fs_source);

    ~Shader();

    uint32_t GetID() const { return id; }
    const std::vector<AttributeDesc>& GetAttributes() const { return attrs; }
    const UniformBlockDesc& GetVSUniformBlock() const { return vsUniformBlock; }
    const std::unordered_map<std::string, int>& GetImageSlots() const { return imageSlots; }

private:
    uint32_t id = 0;
    std::vector<AttributeDesc> attrs;
    UniformBlockDesc vsUniformBlock;
    std::vector<std::string> fsImageNames;
    std::unordered_map<std::string, int> imageSlots;
    std::string vsSource;
    std::string fsSource;
};