#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
    enum class RUNTIME_API BlendColorWriteMask : byte
    {
        Red = 1,
        Green = 2,
        Blue = 4,
        Alpha = 8,
        All = Red | Green | Blue | Alpha
    };

    GENERATE_ENUM_FLAG(BlendColorWriteMask, byte);
}