#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
    enum class TextureFormat : u8
    {
        Unknown,
        RGBA32_Float,
        RGBA32_UInt,
        RGBA32_Int,

        RGB32_Float,
        RGB32_UInt,
        RGB32_Int,

        RGBA16_Float,
        RGBA16_UNorm,
        RGBA16_UInt,
        RGBA16_Norm,
        RGBA16_Int,

        RG32_Float,
        RG32_UInt,
        RG32_Int,

        RGBA8_UNorm,
        RGBA8_UInt,
        RGBA8_Norm,
        RGBA8_Int,
        RGBA8_SRGB,

        RG16_Float,
        RG16_UNorm,
        RG16_UInt,
        RG16_Norm,
        RG16_Int,

        D32_Float,
        R32_Float,
        R32_UInt,
        R32_Int,

        D24_UNorm_S8_UInt,
        D32_Float_S8_UInt,

        RG8_UNorm,
        RG8_UInt,
        RG8_Norm,
        RG8_Int,

        R16_Float,
        D16_UNorm,
        R16_UNorm,
        R16_UInt,
        R16_Norm,
        R16_Int,

        R8_UNorm,
        R8_UInt,
        R8_Norm,
        R8_Int,
        A8_UNorm
    };
}