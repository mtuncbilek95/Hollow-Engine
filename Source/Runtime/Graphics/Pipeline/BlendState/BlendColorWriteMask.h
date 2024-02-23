#pragma once
#include <Runtime/Core/Core.h>

namespace Hollow
{
    enum class RUNTIME_API BlendColorWriteMask : unsigned char
    {
        R = 1,
        G = 2,
        B = 4,
        A = 8,
        All = R | G | B | A
    };
}
