#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/DepthStencilState/StencilOperation.h>
#include <Runtime/Graphics/Common/CompareOperation.h>

namespace Hollow
{
    struct RUNTIME_API StencilFaceDesc
    {
        StencilOperation FailOperation;
        StencilOperation PassOperation;
        StencilOperation DepthFailOperation;
        CompareOperation CompareOperation;
        uint32 CompareMask;
        uint32 WriteMask;
        uint32 Reference;
    };
}