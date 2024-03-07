#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
    class DescriptorSetLayout;
    struct RUNTIME_API ResourceLayoutDesc
    {
        Array<DescriptorSetLayout*> ResourceLayouts;
    };
}