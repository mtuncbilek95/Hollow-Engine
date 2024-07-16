#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Core/CompareOperation.h>
#include <Engine/Graphics/Pipeline/DepthStencilState/StencilFaceDesc.h>

namespace Hollow
{
    struct ENGINE_API DepthStencilStateDesc
    {
        bool bDepthTestEnabled;
        bool bDepthWriteEnabled;
        bool bStencilTestEnabled;
        CompareOperation DepthTestOperation;
        StencilFaceDesc StencilFrontFace;
        StencilFaceDesc StencilBackFace;
    };
}