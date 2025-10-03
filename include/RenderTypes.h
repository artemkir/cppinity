#pragma once

#ifndef RENDER_TYPES_H
#define RENDER_TYPES_H

enum class VertexFormat : int {
    INVALID = 0,
    FLOAT = 1,  // SG_VERTEXFORMAT_FLOAT
    FLOAT2 = 2,  
    FLOAT3 = 3,
    FLOAT4 = 4,

    //TODO: add more
};

enum class UniformType : int {
    INVALID = 0,
    FLOAT = 1,   // SG_UNIFORMTYPE_FLOAT
    FLOAT2 = 2,
    FLOAT3 = 3,
    FLOAT4 = 4,
};

enum class IndexType : int {
    NONE = 1,
    UINT16 = 2,  // SG_INDEXTYPE_UINT16
};

class RenderTypeUtils
{
public:
    static int GetSize(UniformType type) {
        switch (type) {
        case UniformType::FLOAT: return 4;
        case UniformType::FLOAT2: return 8;
        case UniformType::FLOAT3: return 12;
        case UniformType::FLOAT4: return 16;
        default: return 0;
        }
    }

    static int GetAlignment(UniformType type) {
        // Std140 rules: float align 4, vec2 8, vec3/vec4 16
        switch (type) {
        case UniformType::FLOAT: return 4;
        case UniformType::FLOAT2: return 8;
        case UniformType::FLOAT3:
        case UniformType::FLOAT4: return 16;
        default: return 4;
        }
    }

    static int GetComponentCount(UniformType type) {
        switch (type) {
        case UniformType::FLOAT: return 1;
        case UniformType::FLOAT2: return 2;
        case UniformType::FLOAT3: return 3;
        case UniformType::FLOAT4: return 4;
        default: return 0;
        }
    }
};

#endif