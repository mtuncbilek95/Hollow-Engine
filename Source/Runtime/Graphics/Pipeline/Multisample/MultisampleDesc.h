#pragma once

#include <Runtime/Graphics/Texture/TextureSample.h>

namespace Hollow
{
    struct RUNTIME_API MultisampleDesc
    {
        bool bSampleShadingEnabled;
        TextureSample Samples;
    };
}
