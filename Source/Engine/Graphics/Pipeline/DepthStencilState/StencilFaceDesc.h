#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Pipeline/DepthStencilState/StencilOperation.h>
#include <Engine/Graphics/Core/CompareOperation.h>

namespace Hollow
{
    struct StencilFaceDesc
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