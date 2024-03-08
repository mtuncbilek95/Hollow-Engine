#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
    class DescriptorLayout;
    struct RUNTIME_API ResourceLayoutDesc
    {
        Array<DescriptorLayout*> ResourceLayouts;
    };
}