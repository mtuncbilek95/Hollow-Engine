#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Texture/SampleCount.h>

namespace Hollow
{
    struct ENGINE_API MultisampleDesc
    {
        bool bSampleShadingEnabled;
        SampleCount Samples;
    };
}