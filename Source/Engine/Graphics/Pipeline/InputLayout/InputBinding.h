#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Pipeline/InputLayout/InputBindingStepRate.h>
#include <Engine/Graphics/Pipeline/InputLayout/InputElement.h>

namespace Hollow
{
    struct InputBinding
    {
        InputBindingStepRate StepRate;
        DArray<InputElement> Elements;
    };
}