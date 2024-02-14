#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
    enum class RUNTIME_API TextureFormat : byte
    {
        Unknown,
        RGBA32_Typeless,
        RGBA32_Float,
        RGBA32_UInt,
        RGBA32_Int,

        RGB32_Typeless,
        RGB32_Float,
        RGB32_UInt,
        RGB32_Int,

        RGBA16_Typeless,
        RGBA16_Float,
        RGBA16_UNorm,
        RGBA16_UInt,
        RGBA16_Norm,
        RGBA16_Int,

        RG32_Typeless,
        RG32_Float,
        RG32_UInt,
        RG32_Int,

        RGBA8_Typeless,
        RGBA8_UNorm,
        RGBA8_UInt,
        RGBA8_Norm,
        RGBA8_Int,

        RG16_Typeless,
        RG16_Float,
        RG16_UNorm,
        RG16_UInt,
        RG16_Norm,
        RG16_Int,

        R32_Typeless,
        D32_Float,
        R32_Float,
        R32_UInt,
        R32_Int,

        D24_UNorm_S8_UInt,
        R24_UNorm_X8_Typeless,

        RG8_Typeless,
        RG8_UNorm,
        RG8_UInt,
        RG8_Norm,
        RG8_Int,

        R16_Typeless,
        R16_Float,
        D16_UNorm,
        R16_UNorm,
        R16_UInt,
        R16_Norm,
        R16_Int,

        R8_Typeless,
        R8_UNorm,
        R8_UInt,
        R8_Norm,
        R8_Int,
        A8_UNorm
    };
}
