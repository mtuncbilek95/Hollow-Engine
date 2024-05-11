#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Core/CompareOperation.h>
#include <Runtime/Graphics/Pipeline/DepthStencilState/StencilFaceDesc.h>

namespace Hollow
{
    struct RUNTIME_API DepthStencilStateDesc
    {
        bool bDepthTestEnabled;
        bool bDepthWriteEnabled;
        bool bStencilTestEnabled;
        CompareOperation DepthTestOperation;
        StencilFaceDesc StencilFrontFace;
        StencilFaceDesc StencilBackFace;
    };
}