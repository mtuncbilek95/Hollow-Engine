#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
    enum class RUNTIME_API InputElementSemantic : byte
    {
        Position,
        Normal,
        Tangent,
        Binormal,
        Color,
        TexCoord
    };
}