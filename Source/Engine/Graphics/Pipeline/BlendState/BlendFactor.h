#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
    enum class ENGINE_API BlendFactor : u8
    {
        Zero,
        One,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        ConstantColor,
        OneMinusConstantColor,
        ConstantAlpha,
        OneMinusConstantAlpha,
        SrcAlphaSat,
        Src1Color,
        OneMinusSrc1Color,
        Src1Alpha,
        OneMinusSrc1Alpha
    };
}