#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
    enum class RUNTIME_API BlendColorWriteMask : byte
    {
        R = 1,
        G = 2,
        B = 4,
        A = 8,
        All = R | G | B | A
    };

    GENERATE_ENUM_FLAG(BlendColorWriteMask, byte);
}
