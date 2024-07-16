#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
    enum class ENGINE_API InputElementSemantic : u8
    {
        Position,
        Normal,
        Tangent,
        BiTangent,
        Color,
        TexCoord
    };
}