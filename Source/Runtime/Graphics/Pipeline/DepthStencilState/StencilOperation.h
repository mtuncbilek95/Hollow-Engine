#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
    enum class RUNTIME_API StencilOperation : byte
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