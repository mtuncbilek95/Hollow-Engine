#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Descriptor/DescriptorLayout.h>

namespace Hollow
{
    struct ENGINE_API ResourceLayoutDesc
    {
        DArray<SharedPtr<DescriptorLayout>> ResourceLayouts;
    };
}