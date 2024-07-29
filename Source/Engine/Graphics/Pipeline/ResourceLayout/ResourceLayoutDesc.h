#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Descriptor/DescriptorLayout.h>

namespace Hollow
{
    struct ResourceLayoutDesc
    {
        DArray<SharedPtr<DescriptorLayout>> ResourceLayouts;
    };
}