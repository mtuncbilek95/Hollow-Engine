#pragma once
#include <Runtime/Core/Core.h>

namespace Hollow
{
    enum class RUNTIME_API BlendOperation : unsigned char
    {
        Add,
        Subtract,
        Max,
        Min,
        ReverseSubtract
    };
}
