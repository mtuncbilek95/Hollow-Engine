#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/InputLayout/MeshTopology.h>
#include <Runtime/Graphics/Pipeline/InputLayout/InputBinding.h>

namespace Hollow
{
    struct RUNTIME_API InputLayoutDesc
    {
        MeshTopology Topology;
        DArray<InputBinding> Bindings;
    };
}