#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
    enum class InputElementSemantic : u8
    {
        Position,
        Normal,
        Tangent,
        BiTangent,
        Color,
        TexCoord
    };
}