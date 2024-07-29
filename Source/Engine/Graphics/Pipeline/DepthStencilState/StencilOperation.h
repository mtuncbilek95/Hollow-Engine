#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
    enum class StencilOperation : u8
    {
        Keep,
        Zero,
        Replace,
        IncrementAndClamp,
        DecrementAndClamp,
        Invert,
        IncrementAndWrap,
        DecrementAndWrap
    };
}