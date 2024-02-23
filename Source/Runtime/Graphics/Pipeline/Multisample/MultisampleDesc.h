#pragma once

#include <Runtime/Graphics/Texture/TextureSampleCount.h>

namespace Hollow
{
    struct RUNTIME_API MultisampleDesc
    {
        bool bSampleShadingEnabled;
        TextureSampleCount Samples;
    };
}
