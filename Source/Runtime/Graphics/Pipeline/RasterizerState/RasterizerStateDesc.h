#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/RasterizerState/PolygonMode.h>
#include <Runtime/Graphics/Pipeline/RasterizerState/FaceCullMode.h>

namespace Hollow
{
    struct RUNTIME_API RasterizerStateDesc
    {
        bool bDepthBiasEnabled;
        float DepthBiasClamp;
        float DepthBiasFactor;
        float DepthBiasSlope;
        PolygonMode FillMode;
        FaceCullMode CullFlags;
        bool bFrontCounterClockwise;
    };
}