#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Texture/SampleCount.h>

namespace Hollow
{
    struct MultisampleDesc
    {
        bool bSampleShadingEnabled;
        SampleCount Samples;
    };
}