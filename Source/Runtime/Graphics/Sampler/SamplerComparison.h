#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
    enum class RUNTIME_API SamplerComparison : byte
    {
        Never,
        Less,
        Equal,
        Less_Equal,
        Greater,
        NotEqual,
        Greater_Equal,
        Always
    };
}
