#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
    enum class ENGINE_API BlendOperation : u8
    {
        Add,
        Subtract,
        Max,
        Min,
        ReverseSubtract
    };
}