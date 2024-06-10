#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/DepthStencilState/StencilOperation.h>
#include <Runtime/Graphics/Core/CompareOperation.h>

namespace Hollow
{
    struct RUNTIME_API StencilFaceDesc
    {
        StencilOperation FailOperation;
        StencilOperation PassOperation;
        StencilOperation DepthFailOperation;
        CompareOperation CompareOperation;
        u32 CompareMask;
        u32 WriteMask;
        u32 Reference;
    };
}