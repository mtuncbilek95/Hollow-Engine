#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/InputLayout/InputBindingStepRate.h>
#include <Runtime/Graphics/Pipeline/InputLayout/InputElement.h>

namespace Hollow
{
    struct RUNTIME_API InputBinding
    {
        InputBindingStepRate StepRate;
        arrayList<InputElement> Elements;
    };
}