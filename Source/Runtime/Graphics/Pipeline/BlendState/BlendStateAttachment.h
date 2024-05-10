#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendFactor.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendOperation.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendColorWriteMask.h>

namespace Hollow
{
    struct RUNTIME_API BlendStateAttachment
    {
        bool bEnabled;
        BlendFactor SourceColorFactor;
        BlendFactor DestinationColorFactor;
        BlendOperation ColorOperation;
        BlendFactor SourceAlphaFactor;
        BlendFactor DestinationAlphaFactor;
        BlendOperation AlphaOperation;
        BlendColorWriteMask WriteMask;
    };
}